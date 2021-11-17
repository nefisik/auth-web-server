#include "methods/include/AdminMethods.hpp"

AdminMethods::AdminMethods() = default;
AdminMethods::~AdminMethods() = default;

void AdminMethods::updateData(const std::string username, const std::string field, const std::string newData)
{
    UpdateData data;
    data.username = username;
    data.field = field;
    data.newData = newData;

    if(!mongo.identification(data.username))
    {
        throw Poco::InvalidArgumentException("Пользователя с данным логином не существует");
    }

    if (((field == FrontData::status) & ((newData != MongoData::params::STATUS_USER) & (newData != MongoData::params::STATUS_ADMIN))) ||
        ((field == FrontData::verification) & ((newData != MongoData::params::VERIFICATION_TRUE) & newData != MongoData::params::VERIFICATION_FALSE)))
    {
        throw Poco::InvalidArgumentException("field '" + field + "' doesn't contain '" + newData + "' field");
    }

    if (data.field == FrontData::password)
    {
        data.field = MongoData::hashPassword;
        data.newData = Algo::sha256(data.newData);
    }

    if (data.field == FrontData::username & data.newData != username & mongo.identification(data.newData))
    {
        throw Poco::InvalidArgumentException("Пользователь с данным логином уже существует");
    }

    if(data.field == FrontData::mail)
    {
        mongo.checkMail(newData, username);
    }

    mongo.adminUpdateData(data);
}

std::string AdminMethods::getAllUsers()
{
    auto users = mongo.adminGetAllUsers();

    new_json["RESPONSE"]["STATUS"] = "200";
    new_json["RESPONSE"]["COMMENT"] = "OK";
    auto result = users_to_json(new_json, users);

    return result;
}

std::string AdminMethods::getAllUnverifiedUsers()
{
    auto users = mongo.adminGetAllUnverifiedUsers();

    new_json["RESPONSE"]["STATUS"] = "200";
    new_json["RESPONSE"]["COMMENT"] = "OK";
    auto result = users_to_json(new_json, users);

    return result;
}

std::string AdminMethods::getAllUnverifiedMailUsers()
{
    auto users = mongo.adminGetAllUnverifiedMailUsers();

    new_json["RESPONSE"]["STATUS"] = "200";
    new_json["RESPONSE"]["COMMENT"] = "OK";
    auto result = users_to_json(new_json, users);

    return result;
}

std::string AdminMethods::getAllAdmins()
{
    auto users = mongo.adminGetAllAdmins();

    new_json["RESPONSE"]["STATUS"] = "200";
    new_json["RESPONSE"]["COMMENT"] = "OK";
    auto result = users_to_json(new_json, users);

    return result;
}

std::string AdminMethods::searchUser(const std::string username)
{
    auto users = mongo.adminGetAllUsers();
    auto resultVec = Algo::searchResult(users, username);

    new_json["RESPONSE"]["STATUS"] = "200";
    new_json["RESPONSE"]["COMMENT"] = "OK";
    auto result = users_to_json(new_json, resultVec);

    return result;
}

void AdminMethods::addUser(const std::string username, const std::string password, const std::string mail)
{
    User user;
    user.username = username;
    user.password = password;
    user.mail = mail;

    if(mongo.identification(user.username))
    {
        throw Poco::InvalidArgumentException("Пользователь с данным логином уже существует");
    }

    mongo.checkMail(mail, username);

    std::string msg = "Для подтверждения регистрации в системе КВиВДИ пользователя "
        + user.username
        + " перейдите по ссылке ниже:\n\nhttp://"
        + MessageLink::signUpVerifyLink
        + Auth::createSignUpToken(user);

    Mail send(mail, MessageTitle::SIGN_UP_VERIFICATION, msg);

    user.hashPassword = Algo::sha256(user.password);
    mongo.adminAddUser(user);
}

void AdminMethods::addAdmin(const std::string username, const std::string password, const std::string mail)
{
    User user;
    user.username = username;
    user.password = password;
    user.mail = mail;

    if(mongo.identification(user.username))
    {
        throw Poco::InvalidArgumentException("Пользователь с данным логином уже существует");
    }

    mongo.checkMail(mail, username);

    std::string msg = "Для подтверждения регистрации в системе КВиВДИ пользователя "
        + user.username
        + " перейдите по ссылке ниже:\n\nhttp://"
        + MessageLink::signUpVerifyLink
        + Auth::createSignUpToken(user);

    Mail send(mail, MessageTitle::SIGN_UP_VERIFICATION, msg);

    user.hashPassword = Algo::sha256(user.password);
    mongo.adminAddAdmin(user);
}

void AdminMethods::deleteUser(const std::string username)
{
    if(!mongo.identification(username))
    {
        throw Poco::InvalidArgumentException("Пользователя с данным логином не существует");
    }

    mongo.adminDeleteUser(username);
}


std::string AdminMethods::users_to_json(ArduinoJson::StaticJsonDocument<1024 * 1024 * 1024>& new_json, const std::vector<User>& users)
{
    std::string result;

    for(int i = 0; i < users.size(); ++i)
    {
        new_json["RESPONSE"]["DATA"][i][FrontData::username] = users.at(i).username;
        new_json["RESPONSE"]["DATA"][i][FrontData::mail] = users.at(i).mail;
        new_json["RESPONSE"]["DATA"][i][FrontData::password] = "*****";
        new_json["RESPONSE"]["DATA"][i][FrontData::status] = users.at(i).status;
        new_json["RESPONSE"]["DATA"][i][FrontData::verification] = users.at(i).verification;
        new_json["RESPONSE"]["DATA"][i][FrontData::mailVerification] = users.at(i).mailVerification;
    }

    ArduinoJson::serializeJson(new_json, result);

    return result;
}