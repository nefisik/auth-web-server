#include "factory/include/factory.h"
#include "handlers/include/LogInRequestHandler.h"
#include "handlers/include/LogOutRequestHandler.h"
#include "Poco/URI.h"

HTTPRequestHandler *HelloRequestHandlerFactory::createRequestHandler(const HTTPServerRequest &request)
{
	Poco::URI uri(request.getURI());
	try
	{
		if (uri.getPath() == "/auth/login/")
		{
			return new LogInRequestHandler();
		}
		if (uri.getPath() == "/auth/logout/")
		{
			return new LogOutRequestHandler();
		}
	}
	catch (std::exception &ex)
	{
		ex.what();
	}
	return nullptr;
}