#include "server-run/include/server.h"
#include "../include/factory.h"
#include "handlers/include/handlers.h"

HTTPRequestHandler *HelloRequestHandlerFactory::createRequestHandler(const HTTPServerRequest &request)
{
	Poco::URI uri(request.getURI());
	if (uri.getPath() == "/hello/")
	{
		return new HelloRequestHandler();
	}
	if (uri.getPath() == "/html/")
	{
		return new HTMLRequestHandler;
	}
	else return nullptr;
}