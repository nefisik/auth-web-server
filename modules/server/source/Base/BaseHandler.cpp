#include "server/include/Base/BaseHandler.hpp"

void BaseHandler::authorizationUser(Request& request, Response& response) const
{
    if (request.getMethod() == HttpRequest::HTTP_OPTIONS)
    {
        response.setStatus(Status::HTTP_OK);
        response.set("Access-Control-Allow-Method", "GET, POST");
        response.set("Access-Control-Allow-Headers", "token, Content-Type, Accept");
        response.send();

        printLogs(request, response);
    }
    else
    {
        if (!request.has(FrontData::token))
        {
            throw Poco::Exception("'token' field is missing", 400);
        }

		const auto accessToken = request.get(FrontData::token);

        if (!Auth::checkAccessToken(accessToken, MongoData::params::STATUS_USER))
        {
            throw Poco::Exception("UNAUTHORIZED", 401);
        }
    }
}

void BaseHandler::authorizationAdmin(Request& request, Response& response) const
{
    if (request.getMethod() == HttpRequest::HTTP_OPTIONS)
    {
        response.setStatus(Status::HTTP_OK);
        response.set("Access-Control-Allow-Method", "GET, POST");
        response.set("Access-Control-Allow-Headers", "token, Content-Type, Accept");
        response.send();

        printLogs(request, response);
    }
    else
    {
        if (!request.has(FrontData::token))
        {
            throw Poco::Exception("'token' field is missing", 400);
        }

		const auto accessToken = request.get(FrontData::token);

        if (!Auth::checkAccessToken(accessToken, MongoData::params::STATUS_ADMIN))
        {
            throw Poco::Exception("UNAUTHORIZED", 401);
        }
    }
}

void BaseHandler::printLogs(Request& request, Response& response) const
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

void BaseHandler::sendError(Request& request, Response& response, const int& status, const std::string& msg) const
{
	boost::property_tree::ptree elem;
	elem.put<std::string>("STATUS", std::to_string(status));
	elem.put<std::string>("MESSAGE", msg);

	boost::property_tree::ptree root;
	root.put("Date Time", Poco::DateTimeFormatter().format(Poco::LocalDateTime().timestamp(), "%Y.%n.%d %H:%M:%S"));
	root.put("CONTENT-TYPE", "application/json");
	root.put_child("RESPONSE", elem);

	std::stringstream ss;
	boost::property_tree::json_parser::write_json(ss, root);

    response.setStatus(std::to_string(status));
	Poco::Net::MediaType type("application/json");
	response.setContentType(type);
	std::ostream& out = response.send();
	out << ss.str();
	out.flush();

	printLogs(request, response);
}

void BaseHandler::sendResponse(Request& request, Response& response, const std::string& respString)
{
    if (!respString.empty())
    {
        response.setStatus(Status::HTTP_OK);
        Poco::Net::MediaType type("application/json");
        response.setContentType(type);
        std::ostream& out = response.send();
        out << respString;
    }
    else
    {
        response.setStatus(Status::HTTP_NO_CONTENT);
        response.send();
    }

	printLogs(request, response);
}

boost::property_tree::ptree BaseHandler::getJson(Request& request)
{
    BoostJSON json;
    std::string data;
    auto& stream = request.stream();
    getline(stream, data);
    std::stringstream ss;
    ss << data;
    if (!data.empty())
    {
        boost::property_tree::read_json(ss, json);
    }

    return json;
}

std::string BaseHandler::getHeader(Request& request, const std::string& header)
{
    if (!request.has(header))
    {
        throw Poco::Exception("'" + header + "' header is missing", 400);
    }

    const std::string data = request.get(header);

    return data;
}

std::string BaseHandler::getField(const BoostJSON& json, const std::string& field)
{
    std::string data;

    for (const auto& it : json)
    {
        if (it.first == field)
        {
            data = it.second.get_value<std::string>();

            if (data.empty())
            {
                throw Poco::Exception("Поле '" + field + "' не заполнено", 400);
            }
        }
    }

    if (data.empty())
    {
        throw Poco::Exception("'" + field + "' field is missing", 400);
    }

    return data;
}

std::string BaseHandler::getUrlData(const std::string& now_url, const std::string& valid_url)
{
    std::string data = now_url.substr(valid_url.size(), now_url.size());

    return data;
}

std::string BaseHandler::getUsernameFromToken(Request& request)
{
    if (!request.has(FrontData::token))
    {
        throw Poco::Exception("'" + FrontData::token + "' header is missing", 400);
    }

    const std::string recoveryToken = request.get(FrontData::token);

    std::string username = Auth::checkRecoveryToken(recoveryToken);

    return username;
}