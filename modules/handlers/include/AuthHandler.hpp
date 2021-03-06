#pragma once

#include "methods/include/AuthMethods.hpp"
#include "server/include/Base/BaseHandler.hpp"

class AuthHandler : public BaseHandler
{
	using Request = Poco::Net::HTTPServerRequest;
	using Response = Poco::Net::HTTPServerResponse;
    using Status = Poco::Net::HTTPResponse::HTTPStatus;
    using BoostJSON = boost::property_tree::ptree;
    using HttpRequest = Poco::Net::HTTPRequest;

private:
    AuthCommands command;
    AuthMethods method;
public:
    AuthHandler(const AuthCommands& _command);
    ~AuthHandler();
    void handleRequest(Request& request, Response& response);
};