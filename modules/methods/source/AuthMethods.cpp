#include "methods/include/AuthMethods.hpp"

void AuthMethods::SIGN_UP(User user)
{
    Poco::MongoDB::Connection connection(MongoConfig::host, MongoConfig::port);
    MongoConnect::sendAuth(connection, MongoConfig::password);

    if (MongoConnect::identification(user.username, connection) == true)
        throw Poco::InvalidArgumentException("User with this login already exists");

    user.hashPassword = Auth::sha256(user);
    MongoConnect::addNewUser(user, connection);
}

std::pair<std::string, std::string> AuthMethods::SIGN_IN(User user)
{
    Poco::MongoDB::Connection connection(MongoConfig::host, MongoConfig::port);
    MongoConnect::sendAuth(connection, MongoConfig::password);
    Poco::Redis::Client redis;
    redis.connect(RedisConfig::host, RedisConfig::port);
    Redis::sendAuth(redis, RedisConfig::password);

    if (MongoConnect::identification(user.username, connection) == false)
        throw Poco::Net::NotAuthenticatedException("User does not exist");

    auto userHashPassword = MongoConnect::getUserHashPassword(user.username, connection);
    user.hashPassword = Auth::sha256(user);

    if (userHashPassword == user.hashPassword)
    {
        user.refreshToken = Auth::create_refresh_token(user);
        user.accessToken = Auth::create_access_token(user);

        Redis::set(redis, user.refreshToken, user.accessToken);
        Redis::expire(redis, user.refreshToken, JWTparams::accessTokenLifetimeSeconds);

        std::pair<std::string, std::string> tokens = std::make_pair(user.refreshToken, user.accessToken);

        return tokens;
    }
    else
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
}

void AuthMethods::SIGN_OUT(std::string refreshToken)
{
    Poco::Redis::Client redis;
    redis.connect(RedisConfig::host, RedisConfig::port);
    Redis::sendAuth(redis, RedisConfig::password);

    Redis::del(redis, refreshToken);
}

std::string AuthMethods::REFRESH(std::string refreshToken)
{
    Poco::Redis::Client redis;
    redis.connect(RedisConfig::host, RedisConfig::port);
    Redis::sendAuth(redis, RedisConfig::password);

    if (!Auth::check_refresh_token(refreshToken))
        throw Poco::Net::NotAuthenticatedException("Unauthorized");

    auto accessToken = Redis::get(redis, refreshToken);
    if (Auth::check_access_token(accessToken))
    {
        accessToken = Auth::create_access_token(refreshToken);
        Redis::set(redis, refreshToken, accessToken);
        Redis::expire(redis, refreshToken, JWTparams::accessTokenLifetimeSeconds);
    }
    else
        Redis::del(redis, refreshToken);

    return accessToken;
}