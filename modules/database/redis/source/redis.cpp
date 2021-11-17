#include "database/redis/include/redis.hpp"

Redis::Redis()
{
    redis.connect(RedisConfig::host, RedisConfig::port);
    sendAuth(RedisConfig::password);
}

void Redis::sendAuth(const std::string& pwd)
{
	Poco::Redis::Array cmd;
	cmd << "AUTH" << pwd;
	redis.execute<std::string>(cmd);
}

void Redis::set(const std::string& refreshToken, const std::string& accessToken)
{
	const auto setCmd = Poco::Redis::Command::set(refreshToken, accessToken);
    if (redis.execute<std::string>(setCmd) != "OK")
    {
        throw Poco::ApplicationException("Redis set tokens failed");
    }
}

void Redis::expire(const std::string& refreshToken, const int& time)
{
    const auto expCmd = Poco::Redis::Command::expire(refreshToken, JWTparams::refreshTokenLifetimeSeconds);
    if(!redis.execute<Poco::Int64>(expCmd))
    {
        throw Poco::ApplicationException("Redis set expire tokens failed");
    }
}

void Redis::del(const std::string& refreshToken)
{
    const auto delCmd = Poco::Redis::Command::del(refreshToken);
    if (!redis.execute<Poco::Int64>(delCmd))
    {
        throw Poco::Net::NotAuthenticatedException();
    }
}

std::string Redis::get(const std::string& refreshToken)
{
    auto gCmd = Poco::Redis::Command::get(refreshToken);
    auto accessToken = redis.execute<Poco::Redis::BulkString>(gCmd);
    if (accessToken.isNull())
    {
        throw Poco::Net::NotAuthenticatedException();
    }
	
	return accessToken.value();
}