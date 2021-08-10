#include "handlers/include/SignInRequestHandler.h"

// curl -l -X POST -H "Connect-Type: application/json" -d '{"username":"kalikov","password":"sdwef842ng59n5g"}' http://localhost:8080/auth/login/

void SignInRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
	Application &app = Application::instance();
	app.logger().information("Request from %s", request.clientAddress().toString());

	User user;
	MongoConnect conn;

	// GET user data
	try
	{
		if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
		{
			boost::property_tree::ptree pt;
			boost::property_tree::read_json(request.stream(), pt);
			boost::property_tree::ptree::const_iterator it = pt.begin();
			if (it->first == "username")
			{
				user.username = it->second.get_value<std::string>();
			}
			if (it->first == "password")
			{
				user.password = it->second.get_value<std::string>();
			}
		}
	}
	catch (Poco::Exception &exc1)
	{
		response.setStatus(Poco::Net::HTTPServerResponse::HTTP_METHOD_NOT_ALLOWED); // error 405
		response.send();
		return;
	}

	// authentication & authorization, or send JWT token to client
	try
	{
		if (conn.authentication(user))
		{
			user.token = conn.getUserToken(user);
			if (user.token == "NULL")
			{
				if (user.hashPassword == conn.getUserHashPassword(user))
				{
					user.token = "smfafisf3h872fhn4"; // generate token
					conn.updateUserToken(user);
				}
				else
				{
					response.setStatus(Poco::Net::HTTPServerResponse::HTTP_UNAUTHORIZED); // error 401
					response.send();
					return;
				}
			}
			else
			{
				response.setStatus(Poco::Net::HTTPServerResponse::HTTP_OK);
				response.add(user.username, user.token);
				response.redirect("http://localhost:8080/hello/");
				response.send();
				return;
			}
		}
		else
		{
			response.setStatus(Poco::Net::HTTPServerResponse::HTTP_UNAUTHORIZED); // error 401
			response.send();
			return;
		}
	}
	catch (const Poco::Exception &exc2)
	{
		response.setStatus(Poco::Net::HTTPServerResponse::HTTP_INTERNAL_SERVER_ERROR); // error 501
		response.send();
		return;
	}
}
