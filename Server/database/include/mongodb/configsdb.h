#pragma once

#include <string>
#include "server-run/preloader.h"

namespace MongoConfig
{
    const std::string host = getDbConfig("MongoDbHost");
    const int port = stoi(getDbConfig("MongoDbPort"));
    const std::string user = getDbConfig("MongoDbUser");
    const std::string password = getDbConfig("MongoDbPassword");
    const std::string connectionString = getDbConfig("MongoDbConnectionString");
    const std::string DbName = getDbConfig("MongoDbName");
    const std::string CollectionName = getDbConfig("MongoDbCollectionName");
} // MongoConfig
