#include "factory/include/factory.h"
#include "handlers/include/SignUpRequestHandler.h"
#include "handlers/include/SignInRequestHandler.h"
#include "handlers/include/SignOutRequestHandler.h"
#include "handlers/include/HelloRequestHandler.h"
#include "handlers/include/AuthRefreshTokenHandler.h"
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
		else if (uri.getPath() == "/auth/signin/")
		{
			return new SignInRequestHandler();
		}
		else if (uri.getPath() == "/auth/signout/")
		{
			return new SignOutRequestHandler();
		}
		else if (uri.getPath() == "/auth/refresh/")
		{
			return new AuthRefreshTokenHandler();
		}
	}
	catch (std::exception &ex)
	{
		ex.what();
	}
	return nullptr;
}