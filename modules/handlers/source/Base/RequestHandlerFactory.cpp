#include "handlers/include/Base/RequestHandlerFactory.hpp"
#include "handlers/include/Base/urls.hpp"
#include "handlers/include/SignUpRequestHandler.hpp"
#include "handlers/include/SignInRequestHandler.hpp"
#include "handlers/include/SignOutRequestHandler.hpp"
#include "handlers/include/AuthRefreshTokenHandler.hpp"
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