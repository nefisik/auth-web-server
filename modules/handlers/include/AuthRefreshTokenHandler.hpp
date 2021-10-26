#pragma once

#include "handlers/include/Base/BaseHandler.hpp"

class AuthRefreshTokenHandler : public BaseHandler
{
public:
    AuthRefreshTokenHandler() = default;

    virtual ~AuthRefreshTokenHandler() = default;

    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};