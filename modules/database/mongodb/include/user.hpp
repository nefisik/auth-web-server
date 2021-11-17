#pragma once

#include <string>

struct User
{
    std::string username;
    std::string password;
    std::string hashPassword;
    std::string accessToken;
    std::string refreshToken;
    std::string mail;
    std::string status;
    std::string verification;
    std::string mailVerification;
};

struct UpdateData
{
    std::string username;
    std::string field;
    std::string newData;
};