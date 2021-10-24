#pragma once

#include <string>
#include "server-run/include/preloader.h"

namespace MongoData
{
    const std::string username = getConfigValue("MongoDbUserUsername");
    const std::string hashPassword = getConfigValue("MongoDbUserHashPassword");
} // MongoData
