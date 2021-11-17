#pragma once

#include <string>
#include "server/include/preloader.hpp"

namespace MongoData
{
    const std::string username = "username";
    const std::string hashPassword = "hashPassword";
    const std::string status = "status";
    const std::string mail = "mail";
    const std::string verification = "verification";
    const std::string mailVerification = "mailVerification";

    namespace params
    {
        const std::string VERIFICATION_TRUE = "true";
        const std::string VERIFICATION_FALSE = "false";
        const std::string MAIL_VERIFICATION_FALSE = "false";
        const std::string MAIL_VERIFICATION_TRUE = "true";
        const std::string STATUS_ADMIN = "admin";
        const std::string STATUS_USER = "user";

    } // MongoData::params

} // MongoData

namespace FrontData
{
    const std::string username = "username";
    const std::string password = "password";
    const std::string status = "status";
    const std::string mail = "mail";
    const std::string verification = "verification";
    const std::string token = "token";
    const std::string field = "field";
    const std::string newData = "newData";
    const std::string mailVerification = "mailVerification";

} // FrontData