#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/MediaType.h>
#include <Poco/URI.h>

#include "database/mongodb/include/mongo.hpp"
#include "database/redis/include/redis.hpp"
#include "handlers/include/Base/static/urls.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "database/algorithms/include/algorithms.hpp"
#include "database/mongodb/include/mongodata.hpp"
#include "database/mongodb/include/configsdb.hpp"

class BaseHandler : public Poco::Net::HTTPRequestHandler
{
protected:
	//_______AUTHORIZATION_______

	void authorizationUser(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) const;

    //_______RESPONSE_SEND_______

	void printLogs(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) const;

	void sendResponse(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response, const int &status, const std::string &msg) const;

	void sendResponseData(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response, const int &status, const std::string &msg, const std::string &data) const;

	void sendResponseTokens(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response, const int &status, const std::string &msg, const std::string &refresh, const std::string& access) const;

	void sendResponseAccess(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response, const int &status, const std::string &msg, const std::string& access) const;

	void setOptions(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) const;
};
