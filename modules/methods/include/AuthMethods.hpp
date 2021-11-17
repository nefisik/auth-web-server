#pragma once

#include "server/include/Base/libs/ArduinoJson.hpp"
#include "server/include/Base/static/AuthStatic.hpp"
#include "database/algorithms/include/algorithms.hpp"
#include "database/redis/include/redis.hpp"
#include "database/mongodb/include/mongo.hpp"
#include "database/mail/include/mail.hpp"
#include "server/include/Base/BaseJson.hpp"

class AuthMethods : public BaseJson
{
private:
    Mongo mongo;
    Redis redis;
public:
    AuthMethods();
    ~AuthMethods();

    void signUp(const std::string username, const std::string password, const std::string mail);

    void signUpVerify(const std::string signUpToken);

    std::string signIn(const std::string username, const std::string password);

    std::string refresh(const std::string refreshToken);

    void signOut(const std::string refreshToken);

    void mailPasswordRecovery(const std::string mail);

    std::string checkRecoveryToken(const std::string recoveryToken);

    void passwordRecovery(const std::string tokenUsername, const std::string username, const std::string password);
};