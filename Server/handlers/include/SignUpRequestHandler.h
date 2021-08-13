#pragma once

#include "server-run/include/server.h"
#include "database/mongodb/include/mongo.h"
#include "database/mongodb/include/user.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "algorithms/include/jwt.h"

class SignUpRequestHandler: public HTTPRequestHandler
{
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
};