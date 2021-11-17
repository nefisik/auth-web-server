#pragma once

#include "Poco/Net/HTTPServer.h"
#include "server/include/Base/static/urls.hpp"
#include "server/include/Base/static/AuthStatic.hpp"
#include "server/include/Base/static/AdminStatic.hpp"
#include "handlers/include/AuthHandler.hpp"
#include "handlers/include/AdminHandler.hpp"
#include "Poco/URI.h"

class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
    Poco::Net::HTTPRequestHandler * createRequestHandler(const Poco::Net::HTTPServerRequest& request);

private:
    bool methodURI(const std::string &valid_uri, const std::string &now_uri);
};
