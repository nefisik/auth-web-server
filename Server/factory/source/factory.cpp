#include "factory/include/factory.h"
#include "handlers/include/SignUpRequestHandler.h"
#include "handlers/include/SignInRequestHandler.h"
#include "handlers/include/SignOutRequestHandler.h"
#include "handlers/include/HelloRequestHandler.h"
#include "Poco/URI.h"

HTTPRequestHandler *HelloRequestHandlerFactory::createRequestHandler(const HTTPServerRequest &request)
{
	Poco::URI uri(request.getURI());
	try
	{
		if (uri.getPath() == "/auth/signup/")
		{
			return new SignUpRequestHandler();
		}
		if (uri.getPath() == "/auth/signin/")
		{
			return new SignInRequestHandler();
		}
		if (uri.getPath() == "/auth/signout/")
		{
			return new SignOutRequestHandler();
		}
		if (uri.getPath() == "/hello/")
		{
			return new HelloRequestHandler();
		}
	}
	catch (std::exception &ex)
	{
		ex.what();
	}
	return nullptr;
}