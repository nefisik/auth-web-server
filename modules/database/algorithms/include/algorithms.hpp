#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <openssl/sha.h>
#include "jwt/jwt.hpp"
#include "Poco/Net/NetException.h"

#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/Cursor.h"
#include "Poco/MongoDB/Array.h"

#include <Poco/Redis/Client.h>
#include <Poco/Redis/Array.h>
#include <Poco/Redis/Command.h>

#include "database/mongodb/include/user.hpp"
#include "database/mongodb/include/mongodata.hpp"
#include <random>

namespace Auth
{
    std::string create_access_token(const User &user);

    std::string create_access_token(const std::string &refreshToken);

    std::string create_refresh_token(const User &user);

    bool check_refresh_token(const std::string &access_token);

    bool check_access_token(const std::string &access_token);

    std::string sha256(const User &user);

    std::string random_string(std::string::size_type lenght);
}
