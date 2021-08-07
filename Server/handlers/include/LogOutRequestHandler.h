#pragma once

#include "server-run/include/server.h"
#include "database/mongodb/include/mongo.h"
#include "database/mongodb/include/user.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

class LogOutRequestHandler: public HTTPRequestHandler
{
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
};
