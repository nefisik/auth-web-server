#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <openssl/sha.h>
#include "jwt/jwt.hpp"
#include "Poco/Net/NetException.h"

#include <algorithm>
#include <vector>
#include <map>

#include "database/mongodb/include/user.hpp"
#include "database/mongodb/include/mongodata.hpp"
#include <random>

namespace Auth
{
    std::string createAccessToken(const User& user);

    std::string createAccessToken(const std::string& refreshToken);

    std::string createRefreshToken(const User& user);

    std::string createRecoveryToken(const User& user);

    std::string createSignUpToken(const User& user);

    bool checkRefreshToken(const std::string& accessToken);

    bool checkAccessToken(const std::string& accessToken, const std::string& status);

    std::string getUsernameAccess(const std::string& accessToken);

    std::string checkRecoveryToken(const std::string& recoveryToken);

    std::string checkSignUpToken(const std::string& signUpToken);

} // Auth

namespace Algo
{
    int minDistance(std::string word1, std::string word2);

    std::vector<User> searchResult(const std::vector<User>& users, const std::string& search);

    std::string sha256(const std::string& password);

    std::string randomString(std::string::size_type lenght);

} // Algo