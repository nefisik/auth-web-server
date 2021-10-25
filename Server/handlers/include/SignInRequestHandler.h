#pragma once

#include "handlers/include/Base/BaseHandler.h"

class SignInRequestHandler : public BaseHandler
{
public:
    SignInRequestHandler() = default;

    virtual ~SignInRequestHandler() = default;

    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};