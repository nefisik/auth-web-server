#pragma once

#include "server/include/preloader.hpp"
#include <algorithm>

namespace JWTparams
{
    const std::string accessTokenKey = Preloader::getConfigValue("JWTaccessTokenKey");
    const std::string refreshTokenKey = Preloader::getConfigValue("JWTrefreshTokenKey");
    const std::string recoveryTokenKey = Preloader::getConfigValue("JWTrecoveryTokenKey");
    const std::string signUpTokenKey = Preloader::getConfigValue("JWTsignUpTokenKey");
    const std::string username = "username";
    const std::string status = "status";
    const std::string jti = "jti";      // token ID
    const std::string iat = "iat_time"; // token issuance time
    const std::string exp = "exp_time"; // token expiration Time
    const int accessTokenLifetimeMinutes = stoi(Preloader::getConfigValue("JWTaccessTokenLifeTimeMinutes"));
    const int refreshTokenLifetimeSeconds = stoi(Preloader::getConfigValue("JWTrefreshTokenLifeTimeSeconds"));
    const int recoveryTokenLifetimeMinutes = stoi(Preloader::getConfigValue("JWTrecoveryPwdLifeTimeMinutes"));

} // JWTparams