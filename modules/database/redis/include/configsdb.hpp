#pragma once

#include "server-run/include/preloader.hpp"

namespace RedisConfig
{
    const std::string host = getConfigValue("RedisDbHost");
    const int port = stoi(getConfigValue("RedisDbPort"));
    const std::string password = getConfigValue("RedisDbPassword");
} // MongoConfig
