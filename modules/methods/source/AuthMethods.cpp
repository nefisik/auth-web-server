#include "methods/include/AuthMethods.hpp"

AuthMethods::AuthMethods() = default;
AuthMethods::~AuthMethods() = default;

void AuthMethods::signUp(const std::string username, const std::string password, const std::string mail)
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

    user.hashPassword = Algo::sha256(user.password);
    mongo.addUser(user);

    std::string msg = "Для подтверждения регистрации в системе КВиВДИ пользователя "
        + user.username
        + " перейдите по ссылке ниже:\n\nhttp://"
        + MessageLink::signUpVerifyLink
        + Auth::createSignUpToken(user);

    Mail send(mail, MessageTitle::SIGN_UP_VERIFICATION, msg);
}

void AuthMethods::signUpVerify(const std::string signUpToken)
{
    std::string username = Auth::checkSignUpToken(signUpToken);
    mongo.verifyMail(username);
}

std::string AuthMethods::signIn(const std::string username, const std::string password)
{
    User user;
    user.username = username;
    user.password = password;

    if(!mongo.identification(user.username))
    {
        throw Poco::InvalidArgumentException("Неверный логин");
    }

    user.hashPassword = Algo::sha256(user.password);
    mongo.checkHashPassword(user.username, user.hashPassword);
    mongo.checkMailVerification(user.username);
    mongo.checkVerification(user.username);

    user.status = mongo.getStatus(user.username);
    user.refreshToken = Auth::createRefreshToken(user);
    user.accessToken = Auth::createAccessToken(user);

    redis.set(user.refreshToken, user.accessToken);
    redis.expire(user.refreshToken, JWTparams::refreshTokenLifetimeSeconds);

    new_json["RESPONSE"]["STATUS"] = "200";
    new_json["RESPONSE"]["COMMENT"] = "OK";
    new_json["RESPONSE"]["MESSAGE"] = "Sign in success";
    new_json["RESPONSE"]["STATUS"] = user.status;
    new_json["RESPONSE"]["REFRESH"] = user.refreshToken;
    new_json["RESPONSE"]["ACCESS"] = user.accessToken;

    std::string result;
    ArduinoJson::serializeJson(new_json, result);
    return result;
}

std::string AuthMethods::refresh(const std::string refreshToken)
{
    if (!Auth::checkRefreshToken(refreshToken))
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }

    auto accessToken = redis.get(refreshToken);

    if (Auth::checkAccessToken(accessToken, MongoData::params::STATUS_USER))
    {
        accessToken = Auth::createAccessToken(refreshToken);
        redis.set(refreshToken, accessToken);
        redis.expire(refreshToken, JWTparams::refreshTokenLifetimeSeconds);
    }
    else
    {
        redis.del(refreshToken);
    }

    new_json["RESPONSE"]["STATUS"] = "200";
    new_json["RESPONSE"]["COMMENT"] = "OK";
    new_json["RESPONSE"]["MESSAGE"] = "Refresh token success";
    new_json["RESPONSE"]["ACCESS"] = accessToken;

    std::string result;
    ArduinoJson::serializeJson(new_json, result);
    return result;
}

void AuthMethods::signOut(const std::string refreshToken)
{
    redis.del(refreshToken);
}

void AuthMethods::mailPasswordRecovery(const std::string mail)
{
    User user;
    user.username = mongo.getUsername(mail);
    user.status = mongo.getStatus(user.username);

    std::string msg = "Чтобы получить доступ к системе КВиВДИ для пользователя "
        + user.username
        + " перейдите по ссылке ниже для изменения пароля:\n\nhttp://"
        + MessageLink::recoveryPwdLink
        + Auth::createRecoveryToken(user)
        + "\n\nСсылка действительна в течение 12 часов";

    Mail send(mail, MessageTitle::RECOVERY_PASSWORD, msg);
}

std::string AuthMethods::checkRecoveryToken(const std::string recoveryToken)
{
    std::string username = Auth::checkRecoveryToken(recoveryToken);

    new_json["RESPONSE"]["STATUS"] = "200";
    new_json["RESPONSE"]["COMMENT"] = "OK";
    new_json["RESPONSE"]["MESSAGE"] = "Check recovery password success";
    new_json["RESPONSE"]["DATA"]["USERNAME"] = username;
    new_json["RESPONSE"]["DATA"]["RECOVERY"] = recoveryToken;

    std::string result;
    ArduinoJson::serializeJson(new_json, result);
    return result;
}

void AuthMethods::passwordRecovery(const std::string tokenUsername, const std::string username, const std::string password)
{
    User user;
    user.username = username;
    user.password = password;

    if((tokenUsername != username) || !mongo.identification(user.username))
    {
        throw Poco::InvalidArgumentException("Неверный логин");
    }

    user.hashPassword = Algo::sha256(user.password);
    mongo.checkVerification(user.username);

    UpdateData data;
    data.username = username;
    data.field = MongoData::hashPassword;
    data.newData = user.hashPassword;

    mongo.adminUpdateData(data);
}