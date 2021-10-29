#pragma once

#include "handlers/include/Base/BaseHandler.hpp"
#include "handlers/include/Base/static/AuthStatic.hpp"

class AuthRequestHandler : public BaseHandler
{
private:
    AuthCommands command;

public:
    AuthRequestHandler(const AuthCommands &command);

    virtual ~AuthRequestHandler();

    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);

private:
    std::string getToken(Poco::Net::HTTPServerRequest &request);
    User getUserData(Poco::Net::HTTPServerRequest &request);
};