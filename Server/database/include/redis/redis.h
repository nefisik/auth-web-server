#pragma once

#include <Poco/Redis/Client.h>
#include <Poco/Redis/Array.h>

#include <Poco/Redis/Client.h>
#include <Poco/Redis/Command.h>

#include "configsdb.h"
#include "configsJWT.h"

#include <Poco/Net/NetException.h>

namespace Redis
{
    void sendAuth(Poco::Redis::Client& client, const std::string& pwd);
    
    void set(Poco::Redis::Client& redis, std::string& refreshToken, std::string& accessToken);

    void expire(Poco::Redis::Client& redis, std::string& refreshToken, const int& time);

    void del(Poco::Redis::Client& redis, std::string& refreshToken);

    std::string get(Poco::Redis::Client& redis, std::string& refreshToken);
} // Redis