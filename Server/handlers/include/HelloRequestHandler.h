#pragma once

#include "server-run/include/server.h"
#include "database/mongodb/include/mongo.h"

class HelloRequestHandler: public HTTPRequestHandler
{
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
};