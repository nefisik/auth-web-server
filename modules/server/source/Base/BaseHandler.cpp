#include "server/include/Base/BaseHandler.hpp"

void BaseHandler::authorizationUser(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) const
{
    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS)
    {
        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
        response.set("Access-Control-Allow-Method", "GET, POST");
        response.set("Access-Control-Allow-Headers", "token, Content-Type, Accept");
        response.send();

        printLogs(request, response);
    }
    else
    {
        if(!request.has(FrontData::token))
        {
            throw Poco::InvalidArgumentException("'token' field is missing");
        }
        const auto accessToken = request.get(FrontData::token);
        try
        {
            if (Auth::checkAccessToken(accessToken, MongoData::params::STATUS_USER) == false)
            {
                throw Poco::Net::NotAuthenticatedException("UNAUTHORIZED");
            }
        }
        catch(...)
        {
            throw Poco::Net::NotAuthenticatedException("UNAUTHORIZED");
        }
    }
}

void BaseHandler::authorizationAdmin(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) const
{
    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS)
    {
        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
        response.set("Access-Control-Allow-Method", "GET, POST");
        response.set("Access-Control-Allow-Headers", "token, Content-Type, Accept");
        response.send();

        printLogs(request, response);
    }
    else
    {
        if(!request.has(FrontData::token))
        {
            throw Poco::InvalidArgumentException("'token' field is missing");
        }
        const auto accessToken = request.get(FrontData::token);
        try
        {
            if (Auth::checkAccessToken(accessToken, MongoData::params::STATUS_ADMIN) == false)
            {
                throw Poco::Net::NotAuthenticatedException("UNAUTHORIZED");
            }
        }
        catch(...)
        {
            throw Poco::Net::NotAuthenticatedException("UNAUTHORIZED");
        }
    }
}

void BaseHandler::printLogs(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) const
{
	std::cout << std::endl << std::endl
			  << std::setw(17) << std::left << "Date Time:" << Poco::DateTimeFormatter().format(Poco::LocalDateTime().timestamp(), "%Y.%n.%d %H:%M:%S") << std::endl
			  << std::setw(17) << std::left << "CLIENT HOST:" << request.clientAddress().host().toString() << std::endl
			  << std::setw(17) << std::left << "METHOD:" << request.getMethod() << std::endl
			  << std::setw(17) << std::left << "URI:" << request.getURI() << std::endl
			  << std::setw(17) << std::left << "RESPONSE STATUS:" << std::to_string(response.getStatus()) << std::endl
			  << std::setw(17) << std::left << "CONTENT_TYPE:" << response.getContentType() << std::endl
			  << std::endl << std::endl;
}

void BaseHandler::sendResponse(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response, const int &status, const std::string &msg) const
{
	boost::property_tree::ptree elem;
	elem.put<std::string>("STATUS", std::to_string(status));
	elem.put<std::string>("COMMENT", msg);

	boost::property_tree::ptree root;
	root.put("Date Time", Poco::DateTimeFormatter().format(Poco::LocalDateTime().timestamp(), "%Y.%n.%d %H:%M:%S"));
	root.put("CONTENT-TYPE", "application/json");
	root.put_child("RESPONSE", elem);

	std::stringstream ss;
	boost::property_tree::json_parser::write_json(ss, root);

    response.setStatus(std::to_string(status));
	Poco::Net::MediaType type("application/json");
	response.setContentType(type);
	std::ostream &out = response.send();
	out << ss.str();
	out.flush();

	printLogs(request, response);
}

void BaseHandler::sendResponseData(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response, const int &status, const std::string &msg, const std::string &data) const
{
	boost::property_tree::ptree elem;
	elem.put<std::string>("STATUS", std::to_string(status));
	elem.put<std::string>("COMMENT", msg);
	elem.put<std::string>("DATA", data);

	boost::property_tree::ptree root;
	root.put("Date Time", Poco::DateTimeFormatter().format(Poco::LocalDateTime().timestamp(), "%Y.%n.%d %H:%M:%S"));
	root.put("CONTENT-TYPE", "application/json");
	root.put_child("RESPONSE", elem);

	std::stringstream ss;
	boost::property_tree::json_parser::write_json(ss, root);

    response.setStatus(std::to_string(status));
	Poco::Net::MediaType type("application/json");
	response.setContentType(type);
	std::ostream &out = response.send();
	out << ss.str();
	out.flush();

	printLogs(request, response);
}

void BaseHandler::sendResponseTokens(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response, const int &status, const std::string &msg, const std::string &refresh, const std::string& access) const
{
	boost::property_tree::ptree elem;
	elem.put<std::string>("STATUS", std::to_string(status));
	elem.put<std::string>("COMMENT", msg);
	elem.put<std::string>("REFRESH", refresh);
	elem.put<std::string>("ACCESS", access);

	boost::property_tree::ptree root;
	root.put("Date Time", Poco::DateTimeFormatter().format(Poco::LocalDateTime().timestamp(), "%Y.%n.%d %H:%M:%S"));
	root.put("CONTENT-TYPE", "application/json");
	root.put_child("RESPONSE", elem);

	std::stringstream ss;
	boost::property_tree::json_parser::write_json(ss, root);

    response.setStatus(std::to_string(status));
	Poco::Net::MediaType type("application/json");
	response.setContentType(type);
	std::ostream &out = response.send();
	out << ss.str();
	out.flush();

	printLogs(request, response);
}

void BaseHandler::sendResponseAccess(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response, const int &status, const std::string &msg, const std::string& access) const
{
	boost::property_tree::ptree elem;
	elem.put<std::string>("STATUS", std::to_string(status));
	elem.put<std::string>("COMMENT", msg);
	elem.put<std::string>("ACCESS", access);

	boost::property_tree::ptree root;
	root.put("Date Time", Poco::DateTimeFormatter().format(Poco::LocalDateTime().timestamp(), "%Y.%n.%d %H:%M:%S"));
	root.put("CONTENT-TYPE", "application/json");
	root.put_child("RESPONSE", elem);

	std::stringstream ss;
	boost::property_tree::json_parser::write_json(ss, root);

    response.setStatus(std::to_string(status));
	Poco::Net::MediaType type("application/json");
	response.setContentType(type);
	std::ostream &out = response.send();
	out << ss.str();
	out.flush();

	printLogs(request, response);
}

void BaseHandler::setOptions(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) const
{
    response.setContentType("application/json");
    response.setKeepAlive(true); 
    response.add("Access-Control-Allow-Headers", CORS);
    response.add("Access-Control-Allow-Method", "POST");
    response.send();

	printLogs(request, response);
}
