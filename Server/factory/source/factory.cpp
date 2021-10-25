#include "factory/include/factory.h"
#include "factory/include/urls.h"
#include "handlers/include/SignUpRequestHandler.h"
#include "handlers/include/SignInRequestHandler.h"
#include "handlers/include/SignOutRequestHandler.h"
#include "handlers/include/AuthRefreshTokenHandler.h"
#include "Poco/URI.h"

Poco::Net::HTTPRequestHandler *RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &request)
{
	Poco::URI uri(request.getURI());

	try
	{
		if (uri.getPath() == AuthSignUpURL)
		{
			return new SignUpRequestHandler();
		}
		else if (uri.getPath() == AuthSignInURL)
		{
			return new SignInRequestHandler();
		}
		else if (uri.getPath() == AuthSignOutURL)
		{
			return new SignOutRequestHandler();
		}
		else if (uri.getPath() == AuthRefreshTokenURL)
		{
			return new AuthRefreshTokenHandler();
		}
		// else
			//error 404
	}
	catch (std::exception &ex)
	{
		ex.what();
	}
	return nullptr;
}