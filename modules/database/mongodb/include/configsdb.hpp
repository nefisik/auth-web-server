#pragma once

#include <string>
#include "server/include/preloader.hpp"
#include <algorithm>

namespace MongoConfig
{
    const std::string host = Preloader::getConfigValue("MongoDbHost");
    const int port = stoi(Preloader::getConfigValue("MongoDbPort"));
    const std::string user = Preloader::getConfigValue("MongoDbUser");
    const std::string password = Preloader::getConfigValue("MongoDbPassword");
    const std::string connectionString = Preloader::getConfigValue("MongoDbConnectionString");
    const std::string DbAuthName = Preloader::getConfigValue("MongoDbAuthenticationName");
    const std::string DbName = Preloader::getConfigValue("MongoDbName");
    const std::string CollectionName = Preloader::getConfigValue("MongoDbCollectionName");

} // MongoConfig
