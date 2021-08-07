#include "handlers/include/LogOutRequestHandler.h"

void LogOutRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
	Application &app = Application::instance();
	app.logger().information("Request from %s", request.clientAddress().toString());

	User user;

	if(request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
	{
		boost::property_tree::ptree pt;
		boost::property_tree::read_json(request.stream(), pt);
		boost::property_tree::ptree::const_iterator it = pt.begin();
		if(it->first == "username")
		{
			user.username = it->second.get_value<std::string>();
		}
		if(it->first == "password")
		{
			user.password = it->second.get_value<std::string>();
		}
		try
		{
			MongoConnect conn;
			// conn.addNewUser(user);
			int authNumberUsers = conn.authentication(user);
			if(authNumberUsers == 1)
			{
				user.passwordValid = 1;
				user.token = "ifcmifw7439f43f"; //generate token
				conn.updateUserToken(user);
			}
			else if(authNumberUsers > 1)
			{
				std::cerr << "There is more than one user named " + user.username + "in the database" <<std::endl;
				return;
			}
			else
			{
				std::cerr << "Unregistred user" <<std::endl;
				return;
			}
		}
		catch(const Poco::Exception& exc)
		{
			std::cerr << exc.displayText() << std::endl;
		}
		
	}

	// return response.setStatus(Poco::Net::HTTPServerResponse::HTTP_OK);
}
