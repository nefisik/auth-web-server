#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/MediaType.h>
#include <Poco/URI.h>

#include "database/mongodb/include/mongo.hpp"
#include "database/redis/include/redis.hpp"
#include "server/include/Base/static/urls.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "database/algorithms/include/algorithms.hpp"
#include "database/mongodb/include/mongodata.hpp"
#include "database/mongodb/include/configsdb.hpp"

#include <boost/exception/diagnostic_information.hpp> 

class BaseHandler : public Poco::Net::HTTPRequestHandler
{
	using Request = Poco::Net::HTTPServerRequest;
	using Response = Poco::Net::HTTPServerResponse;
    using Status = Poco::Net::HTTPResponse::HTTPStatus;
    using HttpRequest = Poco::Net::HTTPRequest;
    using BoostJSON = boost::property_tree::ptree;

protected:
	//_______AUTHORIZATION_______

	void authorizationUser(Request& request, Response& response) const;

	void authorizationAdmin(Request& request, Response& response) const;

    //_______RESPONSE_SEND_______

	void printLogs(Request& request, Response& response) const;

	void sendError(Request& request, Response& response, const int& status, const std::string& msg) const;

    void sendResponse(Request& request, Response& response, const std::string& respString);

	//________GET_DATA___________

    BoostJSON getJson(Request& request);
    std::string getHeader(Request& request, const std::string& header);
    std::string getField(const BoostJSON& json, const std::string& field);
    std::string getUrlData(const std::string& now_url, const std::string& valid_url);
    std::string getUsernameFromToken(Request& request);
};
