#include "server-run/include/server.h"
#include "../include/handlers.h"

void HelloRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
	Application &app = Application::instance();
	app.logger().information("Request from %s", request.clientAddress().toString());

	response.setChunkedTransferEncoding(true);
	response.setContentType("text/html");

	response.send()
		<< "<html>"
		<< "<head><title>Hello</title></head>"
		<< "<body><h1>Hello from the POCO Web Server</h1></body>"
		<< "</html>";
}

void HTMLRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
	Application &app = Application::instance();
	app.logger().information("Request from %s", request.clientAddress().toString());

	Poco::Net::HTMLForm form(request);
	std::ostream &reply = response.send();

	if (form.has("presented-param"))
	{
		reply << "presented param is " << form.get("presented-param") << std::endl;
		// Also works well
		// reply << "presented param is " << form["presented-param"] << std::endl;
	}

	reply << "dummy param is "
		  << form.get("dummy-param", "default Dummy Param")
		  << std::endl;

	reply << "***************** All parametrs ****************" << std::endl;

	for (auto i : form)
	{
		reply << i.first << "\tis\t" << i.second << std::endl;
	}

	response.setStatus(Poco::Net::HTTPServerResponse::HTTP_OK);
}
