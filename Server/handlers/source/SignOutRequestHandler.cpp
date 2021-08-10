#include "handlers/include/SignOutRequestHandler.h"

void SignOutRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
	Application &app = Application::instance();
	app.logger().information("Request from %s", request.clientAddress().toString());

	User user;

	if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
	{
		boost::property_tree::ptree pt;
		boost::property_tree::read_json(request.stream(), pt);
		boost::property_tree::ptree::const_iterator it = pt.begin();
		if (it->first == "username")
		{
			user.username = it->second.get_value<std::string>();
		}

		try
		{
			MongoConnect conn;

			user.token = "NULL";
			conn.logout(user);
		}
		catch (const Poco::Exception &exc)
		{
			std::cerr << exc.displayText() << std::endl;
		}
	}
}
