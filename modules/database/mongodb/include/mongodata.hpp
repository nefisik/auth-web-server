#pragma once

#include <string>
#include "server-run/include/preloader.hpp"

namespace MongoData
{
    const std::string username = getConfigValue("MongoDbUserUsername");
    const std::string hashPassword = getConfigValue("MongoDbUserHashPassword");
} // MongoData
