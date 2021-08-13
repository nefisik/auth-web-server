#include "handlers/include/SignUpRequestHandler.h"
#include "algorithms/include/sha256.h"

void SignUpRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
	Application &app = Application::instance();
	app.logger().information("Request from %s", request.clientAddress().toString());

	std::cout << "________SIGN_UP________" << std::endl;

	User user;

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
		try
		{
			MongoConnect conn;
			// conn.addNewUser(user);
			// int authNumberUsers = conn.authentication(user);
			if (!conn.authentication(user))
			{
				user.status = MongoData::status;
				sha256(user);
				create_token(user); //generate JWT token
				conn.addNewUser(user);

				response.setStatus(Poco::Net::HTTPServerResponse::HTTP_OK);
			}
			else
			{
				response.setStatus(Poco::Net::HTTPServerResponse::HTTP_UNAUTHORIZED);
				response.send();
				std::cerr << "This username is alredy in use" << std::endl;
			}
		}
		catch (const Poco::Exception &exc)
		{
			std::cerr << exc.displayText() << std::endl;
		}
	}
}
