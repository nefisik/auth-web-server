#pragma once

#include "server-run/include/server.h"

class HelloRequestHandlerFactory: public HTTPRequestHandlerFactory
{
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest&);
};