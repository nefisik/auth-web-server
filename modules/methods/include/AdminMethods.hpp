#pragma once

#include "server/include/Base/libs/ArduinoJson.hpp"
#include "server/include/Base/static/AdminStatic.hpp"
#include "database/algorithms/include/algorithms.hpp"
#include "database/redis/include/redis.hpp"
#include "database/mongodb/include/mongo.hpp"
#include "database/mail/include/mail.hpp"
#include "server/include/Base/BaseJson.hpp"

class AdminMethods : public BaseJson
{
private:
    Mongo mongo;
public:
    AdminMethods();
    ~AdminMethods();

    void updateData(const std::string username, const std::string field, const std::string newData);

    std::string getAllUsers();

    std::string getAllUnverifiedUsers();

    std::string getAllUnverifiedMailUsers();

    std::string getAllAdmins();

    std::string searchUser(const std::string username);

    void addUser(const std::string username, const std::string password, const std::string mail);

    void addAdmin(const std::string username, const std::string password, const std::string mail);

    void deleteUser(const std::string username);

private:
    std::string users_to_json(ArduinoJson::StaticJsonDocument<1024 * 1024 * 1024>& new_json, const std::vector<User>& users);
};