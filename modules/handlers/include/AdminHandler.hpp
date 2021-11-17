#pragma once

#include "methods/include/AdminMethods.hpp"
#include "server/include/Base/BaseHandler.hpp"

class AdminHandler : public BaseHandler
{
private:
    AdminCommands command;
public:
    AdminHandler(const AdminCommands& command);
    ~AdminHandler();
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
private:
    boost::property_tree::ptree getJson(Poco::Net::HTTPServerRequest &request);
    std::string getField(boost::property_tree::ptree &json, const std::string& field);
};