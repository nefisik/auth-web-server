#pragma once

#include "server-run/preloader.h"

namespace RedisConfig
{
    const std::string host = getDbConfig("RedisDbHost");
    const int port = stoi(getDbConfig("RedisDbPort"));
    const std::string password = getDbConfig("RedisDbPassword");
} // MongoConfig
