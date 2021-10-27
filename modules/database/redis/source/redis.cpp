#include "database/redis/include/redis.hpp"

void Redis::sendAuth(Poco::Redis::Client& client, const std::string& pwd)
{
	Poco::Redis::Array cmd;
	cmd << "AUTH" << pwd;
	client.execute<std::string>(cmd);
}

void Redis::set(Poco::Redis::Client& redis, std::string& refreshToken, std::string& accessToken)
{
	auto setCmd = Poco::Redis::Command::set(refreshToken, accessToken);
    if (redis.execute<std::string>(setCmd) != "OK")
        throw Poco::ApplicationException("Redis set tokens failed");
}

void Redis::expire(Poco::Redis::Client& redis, std::string& refreshToken, const int& time)
{
    auto expCmd = Poco::Redis::Command::expire(refreshToken, JWTparams::accessTokenLifetimeSeconds);
    if(!redis.execute<Poco::Int64>(expCmd))
        throw Poco::ApplicationException("Redis set expire tokens failed");
}

void Redis::del(Poco::Redis::Client& redis, std::string& refreshToken)
{
    auto delCmd = Poco::Redis::Command::del(refreshToken);
    if (!redis.execute<Poco::Int64>(delCmd))
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
}

std::string Redis::get(Poco::Redis::Client& redis, std::string& refreshToken)
{
    auto gCmd = Poco::Redis::Command::get(refreshToken);
    auto accessToken = redis.execute<Poco::Redis::BulkString>(gCmd);
    if (accessToken.isNull())
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
	
	return accessToken.value();
}