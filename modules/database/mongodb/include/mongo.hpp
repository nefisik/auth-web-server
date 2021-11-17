#pragma once

#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/Cursor.h"
#include "Poco/MongoDB/Array.h"
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/NetException.h>
#include "user.hpp"
#include "mongodata.hpp"
#include "configsdb.hpp"

class Mongo
{
private:
    Poco::MongoDB::Connection connection;
public:
    Mongo();

    /************************************USER*************************************/

    void sendAuth(const std::string& dbPassword);

    void addUser(const User& user);

    bool identification(const std::string& username);

    void checkMail(const std::string& mail, const std::string& username);

    void checkVerification(const std::string& username);

    void checkMailVerification(const std::string& username);

    void checkHashPassword(const std::string& username, const std::string& hashPassword);

    std::string getStatus(const std::string& username);

    std::string getUsername(const std::string& mail);

    void verifyMail(const std::string& username);

    /************************************ADMIN************************************/

    void adminUpdateData(const UpdateData& data);

    std::vector<User> adminGetAllUsers();

    std::vector<User> adminGetAllUnverifiedUsers();

    std::vector<User> adminGetAllUnverifiedMailUsers();

    std::vector<User> adminGetAllAdmins();

    void adminAddUser(const User& user);

    void adminAddAdmin(const User& user);

    void adminDeleteUser(const std::string& username);
};