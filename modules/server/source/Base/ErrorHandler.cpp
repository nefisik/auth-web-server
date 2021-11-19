#include "server/include/Base/ErrorHandler.hpp"

ErrorHandler::ErrorHandler(const Status& status)
    :status(status) {}

ErrorHandler::~ErrorHandler() = default;

void ErrorHandler::handleRequest(Request& request, Response& response)
{
    response.set("Access-Control-Allow-Method", "GET");

    std::string resp_string;
    response.setStatus(status);
    Poco::Net::MediaType type("application/json");
    response.setContentType(type);
    std::ostream& out = response.send();
    out << resp_string;

	std::cout << std::endl << std::endl
			  << std::setw(17) << std::left << "Date Time:" << Poco::DateTimeFormatter().format(Poco::LocalDateTime().timestamp(), "%Y.%n.%d %H:%M:%S") << std::endl
			  << std::setw(17) << std::left << "CLIENT HOST:" << request.clientAddress().host().toString() << std::endl
			  << std::setw(17) << std::left << "METHOD:" << request.getMethod() << std::endl
			  << std::setw(17) << std::left << "URI:" << request.getURI() << std::endl
			  << std::setw(17) << std::left << "RESPONSE STATUS:" << std::to_string(Poco::Net::HTTPResponse::HTTPStatus(status)) << std::endl
			  << std::setw(17) << std::left << "CONTENT_TYPE:" << response.getContentType() << std::endl
			  << std::endl << std::endl;
}