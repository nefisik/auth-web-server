#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "database/include/algorithms/algorithms.h"
#include "database/include/mongodb/mongodata.h"
#include "database/include/mongodb/configsdb.h"

class SignOutRequestHandler : public HTTPRequestHandler {
public:
    SignOutRequestHandler() = default;

    virtual ~SignOutRequestHandler() = default;

    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};
