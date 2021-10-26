#pragma once

#include <string>
#include "server-run/include/preloader.hpp"

namespace MongoConfig
{
    const std::string host = getConfigValue("MongoDbHost");
    const int port = stoi(getConfigValue("MongoDbPort"));
    const std::string user = getConfigValue("MongoDbUser");
    const std::string password = getConfigValue("MongoDbPassword");
    const std::string connectionString = getConfigValue("MongoDbConnectionString");
    const std::string DbName = getConfigValue("MongoDbName");
    const std::string CollectionName = getConfigValue("MongoDbCollectionName");
} // MongoConfig
