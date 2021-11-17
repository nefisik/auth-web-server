#pragma once

#include "server/include/preloader.hpp"
#include <algorithm>

namespace RedisConfig
{
    const std::string host = Preloader::getConfigValue("RedisDbHost");
    const int port = stoi(Preloader::getConfigValue("RedisDbPort"));
    const std::string password = Preloader::getConfigValue("RedisDbPassword");

} // MongoConfig
