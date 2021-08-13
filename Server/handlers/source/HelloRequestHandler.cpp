#include "handlers/include/HelloRequestHandler.h"

void HelloRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
	try
	{
		MongoConnect conn;

		if(!request.hasToken("token", conn.getUserTokenToken(request.get("token"))))
		{
			response.setStatus(Poco::Net::HTTPServerResponse::HTTP_UNAUTHORIZED);
			response.send();
		}
	}
	catch(Poco::Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
	}
	
	Application &app = Application::instance();
	app.logger().information("Request from %s", request.clientAddress().toString());

	response.setStatus(Poco::Net::HTTPServerResponse::HTTP_OK);
	response.send();
}
