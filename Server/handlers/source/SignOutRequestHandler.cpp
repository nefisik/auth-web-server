#include "handlers/include/SignOutRequestHandler.h"

void SignOutRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
	MongoConnect conn;
	User user;

	request.hasToken("token", conn.getUserTokenToken(request.get("token", user.token)));

	Application &app = Application::instance();
	app.logger().information("Request from %s", request.clientAddress().toString());

	try
	{
		MongoConnect conn;

		user.token = "NULL";
		conn.logout(user.token);
	}
	catch (const Poco::Exception &exc)
	{
		std::cerr << exc.displayText() << std::endl;
	}
}
