#include "handlers/include/Base/RequestHandlerFactory.hpp"

Poco::Net::HTTPRequestHandler *RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &request)
{
	Poco::URI uri(request.getURI());

	try
	{
		if (uri.getPath() == AuthSignUpURL)
		{
			return new AuthRequestHandler(AuthCommands::SIGN_UP);
		}
		else if (uri.getPath() == AuthSignInURL)
		{
			return new AuthRequestHandler(AuthCommands::SIGN_IN);
		}
		else if (uri.getPath() == AuthSignOutURL)
		{
			return new AuthRequestHandler(AuthCommands::SIGN_OUT);
		}
		else if (uri.getPath() == AuthRefreshTokenURL)
		{
			return new AuthRequestHandler(AuthCommands::REFRESH);
		}
		else
		{
			// int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND;
			// std::string msg = "Method not found";
        	// sendResponse(request, response, status, msg);
		}
	}
	catch (...)
	{
		// int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_INTERNAL_SERVER_ERROR;
		// std::string msg = "Internal server error";
        // sendResponse(request, response, status, msg);
	}

	return nullptr;
}