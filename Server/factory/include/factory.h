#pragma once

#include "Poco/Net/HTTPServer.h"
#include "handlers/include/Base/BaseHandler.h"

class RequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
{
    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&);
};