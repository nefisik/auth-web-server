#pragma once

#include "methods/include/AuthMethods.hpp"
#include "server/include/Base/BaseHandler.hpp"

class AuthHandler : public BaseHandler
{
private:
    AuthCommands command;
public:
    AuthHandler(const AuthCommands& _command);
    ~AuthHandler();
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
private:
    boost::property_tree::ptree getJson(Poco::Net::HTTPServerRequest &request);
    std::string getHeader(Poco::Net::HTTPServerRequest& request, const std::string& header);
    std::string getField(const boost::property_tree::ptree& json, const std::string& field);
    std::string getUrlData(const std::string& now_url, const std::string& valid_url);
    std::string getTokenUsername(Poco::Net::HTTPServerRequest& request);
};