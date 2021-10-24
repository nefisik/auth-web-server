#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <openssl/sha.h>
#include "jwt/jwt.hpp"
#include "Poco/Net/NetException.h"

#include "database/include/mongodb/user.h"
#include "database/include/mongodb/mongodata.h"
#include "handlers/include/Base/BaseHandler.h"
#include <random>

namespace Auth
{
    void authorizationUser(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);

    std::string create_access_token(const User &user);

    std::string create_access_token(const std::string &refreshToken);

    std::string create_refresh_token(const User &user);

    bool check_refresh_token(const std::string &access_token);

    bool check_access_token(const std::string &access_token);

    std::string sha256(const User &user);

    std::string random_string(std::string::size_type lenght);
}
