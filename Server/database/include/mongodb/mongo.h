#pragma once

#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/Cursor.h"
#include "Poco/MongoDB/Array.h"
#include <Poco/Net/NetException.h>
#include "user.h"
#include "database/include/mongodb/mongodata.h"
#include "database/include/mongodb/configsdb.h"

namespace MongoConnect
{
    void sendAuth(Poco::MongoDB::Connection&);
    void addNewUser(const User&, Poco::MongoDB::Connection&);
    bool identification(const std::string&, Poco::MongoDB::Connection&);
    std::string getUserHashPassword(const std::string&, Poco::MongoDB::Connection&);

    /*************************************BACKLOG***************************************/
    // void updateUserUsername(const std::string&, Poco::MongoDB::Connection&);
    // void updateUserHashPassword(const User &user, Poco::MongoDB::Connection&);
    // void deleteUser(const std::string&, Poco::MongoDB::Connection&);

    /************************************DEPRECATED*************************************/
    // std::string getUserRefreshToken(const std::string&, Poco::MongoDB::Connection&);
    // std::string getUserAccessToken(const std::string&, Poco::MongoDB::Connection&);
    // void checkRefreshToken(const std::string&, Poco::MongoDB::Connection&);
    // void checkAccessToken(const std::string&, Poco::MongoDB::Connection&);
    // void updateUserRefreshToken(const User&, Poco::MongoDB::Connection&);
    // void updateUserAccessToken(const User&, Poco::MongoDB::Connection&);
    // void updateUserAccessToken(const std::string&, const std::string&, Poco::MongoDB::Connection&);
    // void logout(const std::string&, Poco::MongoDB::Connection&);
}