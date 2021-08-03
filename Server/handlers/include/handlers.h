#pragma once

#include "server-run/include/server.h"

class HelloRequestHandler: public HTTPRequestHandler
{
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
};

class HTMLRequestHandler: public HTTPRequestHandler
{
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
};