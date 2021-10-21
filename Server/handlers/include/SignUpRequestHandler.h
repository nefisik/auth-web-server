#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "database/include/algorithms/algorithms.h"
#include "database/include/mongodb/configsdb.h"

class SignUpRequestHandler : public HTTPRequestHandler {
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};