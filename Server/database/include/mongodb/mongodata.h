#pragma once

#include <string>
#include "server-run/preloader.h"

namespace MongoData
{
    const std::string username = getDbConfig("MongoDbUserUsername");
    const std::string hashPassword = getDbConfig("MongoDbUserHashPassword");
} // MongoData
