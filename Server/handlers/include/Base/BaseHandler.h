#pragma once

#include <memory>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/MediaType.h>
#include <Poco/URI.h>
#include <Poco/Net/NetException.h>

#include "database/include/mongodb/mongo.h"
#include "database/include/redis/redis.h"
#include "factory/include/urls.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "database/include/algorithms/algorithms.h"
#include "database/include/mongodb/mongodata.h"
#include "database/include/mongodb/configsdb.h"

class BaseHandler : public Poco::Net::HTTPRequestHandler
{
protected:
    //_______RESPONSE_SEND_______

	void printLogs(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) const;

	void sendResponse(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response, Poco::Net::HTTPResponse &status, std::string &msg, std::string &data) const;

	void sendResponseData(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response, Poco::Net::HTTPResponse &status, std::string &msg, std::string &data) const;

	void sendResponseTokens(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response, Poco::Net::HTTPResponse &status, std::string &msg, std::string &refresh, std::string& access) const;

	void sendResponseAccess(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response, Poco::Net::HTTPResponse &status, std::string &msg, std::string& access) const;
};
