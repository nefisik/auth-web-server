#pragma once

#include "handlers/include/Base/BaseHandler.h"

class SignOutRequestHandler : public BaseHandler {
public:
    SignOutRequestHandler() = default;

    virtual ~SignOutRequestHandler() = default;

    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};
