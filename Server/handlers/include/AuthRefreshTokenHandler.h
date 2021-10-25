#pragma once

#include "handlers/include/Base/BaseHandler.h"

class AuthRefreshTokenHandler : public BaseHandler
{
public:
    AuthRefreshTokenHandler() = default;

    virtual ~AuthRefreshTokenHandler() = default;

    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};