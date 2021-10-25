#include "handlers/include/Base/BaseHandler.h"
#include <string>

void BaseHandler::authorizationUser(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) const
{
    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS) {
        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
        response.set("Access-Control-Allow-Method", "GET, POST");
        response.set("Access-Control-Allow-Headers", "token, Content-Type, Accept");
        response.send();

        printLogs(request, response);
    }
    else
    {
        auto accessToken = request.get("token");
        if (Auth::check_access_token(accessToken) == false)
            throw Poco::Net::NotAuthenticatedException();
    }
}

void BaseHandler::printLogs(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) const
{
	std::cout << std::endl << std::endl;

	std::cout << "Date Time: \t" + Poco::DateTimeFormatter().format(Poco::LocalDateTime().timestamp(), "%Y.%n.%d %H:%M:%S");
	std::cout << "CLIENT HOST: \t" + request.clientAddress().host().toString() << std::endl;
	std::cout << "METHOD: \t" + request.getMethod() << std::endl;
	std::cout << "URI : \t" + request.getURI() << std::endl;
	std::cout << "RESPONSE STATUS: \t" + std::to_string(response.getStatus()) << std::endl;
	std::cout << "CONTENT_TYPE \t" + response.getContentType() << std::endl;

	std::cout << std::endl << std::endl;
}

void BaseHandler::sendResponse(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response, Poco::Net::HTTPResponse::HTTPStatus &status, std::string &msg, std::string &data) const
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

    response.setStatus(status);
	Poco::Net::MediaType type("application/json");
	response.setContentType(type);
	std::ostream &out = response.send();
	out << ss.str();
	out.flush();

	printLogs(request, response);
}

void BaseHandler::sendResponseData(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response, Poco::Net::HTTPResponse::HTTPStatus &status, std::string &msg, std::string &data) const
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

    response.setStatus(status);
	Poco::Net::MediaType type("application/json");
	response.setContentType(type);
	std::ostream &out = response.send();
	out << ss.str();
	out.flush();

	printLogs(request, response);
}

void BaseHandler::sendResponseTokens(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response, Poco::Net::HTTPResponse::HTTPStatus &status, std::string &msg, std::string &refresh, std::string& access) const
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

    response.setStatus(status);
	Poco::Net::MediaType type("application/json");
	response.setContentType(type);
	std::ostream &out = response.send();
	out << ss.str();
	out.flush();

	printLogs(request, response);
}

void BaseHandler::sendResponseAccess(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response, Poco::Net::HTTPResponse::HTTPStatus &status, std::string &msg, std::string& access) const
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

    response.setStatus(status);
	Poco::Net::MediaType type("application/json");
	response.setContentType(type);
	std::ostream &out = response.send();
	out << ss.str();
	out.flush();

	printLogs(request, response);
}
