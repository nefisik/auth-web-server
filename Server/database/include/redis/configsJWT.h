#pragma once

#include "server-run/include/preloader.h"

namespace JWTparams
{
    const std::string accessTokenKey = getConfigValue("RedisUserAccessTokenKey");
    const std::string refreshTokenKey = getConfigValue("RedisUserRefreshTokenKey");
    const std::string username = getConfigValue("MongoDbUserUsername");
    const std::string jti = "jti";      // token ID
    const std::string iat = "iat_time"; // token issuance time
    const std::string exp = "exp_time"; // token expiration Time
    const int accessTokenLifetimeMinutes = stoi(getConfigValue("MongoDbUserAccessTokenLifeTimeMinutes"));
    const int accessTokenLifetimeSeconds = stoi(getConfigValue("RedisDbUserAccessTokenLifeTimeSeconds"));
} // JWTparams