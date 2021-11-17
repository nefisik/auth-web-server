#pragma once

#include <Poco/Redis/Client.h>
#include <Poco/Redis/Array.h>

#include <Poco/Redis/Client.h>
#include <Poco/Redis/Command.h>

#include "configsdb.hpp"
#include "redisdata.hpp"

#include <Poco/Net/NetException.h>

class Redis
{
private:
    Poco::Redis::Client redis;

public:
    Redis();

    void sendAuth(const std::string& pwd);
    void set(const std::string& refreshToken, const std::string& accessToken);
    void expire(const std::string& refreshToken, const int& time);
    void del(const std::string& refreshToken);
    std::string get(const std::string& refreshToken);

};