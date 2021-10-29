#pragma once

#include "Poco/Net/HTTPServer.h"
#include "handlers/include/Base/static/urls.hpp"
#include "handlers/include/Base/static/AuthStatic.hpp"
#include "handlers/include/AuthHandler.hpp"
#include "Poco/URI.h"

class RequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
{
    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&);
};