#pragma once

#include "server-run/preloader.h"

namespace JWTparams
{
    const std::string accessTokenKey = getDbConfig("RedisUserAccessTokenKey");
    const std::string refreshTokenKey = getDbConfig("RedisUserRefreshTokenKey");
    const std::string username = getDbConfig("MongoDbUserUsername");
    const std::string jti = "jti";      // token ID
    const std::string iat = "iat_time"; // token issuance time
    const std::string exp = "exp_time"; // token expiration Time
    const int accessTokenLifetimeMinutes = stoi(getDbConfig("MongoDbUserAccessTokenLifeTimeMinutes"));
    const int accessTokenLifetimeSeconds = stoi(getDbConfig("RedisDbUserAccessTokenLifeTimeSeconds"));
} // JWTparams