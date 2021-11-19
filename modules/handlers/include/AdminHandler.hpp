#pragma once

#include "methods/include/AdminMethods.hpp"
#include "server/include/Base/BaseHandler.hpp"

class AdminHandler : public BaseHandler
{
	using Request = Poco::Net::HTTPServerRequest;
	using Response = Poco::Net::HTTPServerResponse;
    using Status = Poco::Net::HTTPResponse::HTTPStatus;
    using BoostJSON = boost::property_tree::ptree;
    using HttpRequest = Poco::Net::HTTPRequest;

private:
    AdminCommands command;
    AdminMethods method;
public:
    AdminHandler(const AdminCommands& command);
    ~AdminHandler();
    void handleRequest(Request& request, Response& response);
};