#pragma once

#include "handlers/include/Base/BaseHandler.hpp"

class SignUpRequestHandler : public BaseHandler
{
public:
    SignUpRequestHandler() = default;

    virtual ~SignUpRequestHandler() = default;

    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};