#pragma once

#include <string>

struct User
{
    std::string username;
    std::string password;
    std::string hashPassword;
    std::string accessToken;
    std::string refreshToken;
};