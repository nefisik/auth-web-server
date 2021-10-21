#include "handlers/include/SignUpRequestHandler.h"

void SignUpRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    request.set("Access-Control-Allow-Origin", CORS);
    response.set("Access-Control-Allow-Origin", CORS);
    response.set("Access-Control-Allow-Method", "POST");

    try
    {
        if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
        {
            std::unique_ptr<Poco::URI> uri(new Poco::URI(request.getURI()));

            std::string resp_data = "";
            std::string resp_status = "";

            User user;

            boost::property_tree::ptree pt;
            boost::property_tree::read_json(request.stream(), pt);
            boost::property_tree::ptree::const_iterator it = pt.begin();
            if (it->first == "username")
            {
                user.username = it->second.get_value<std::string>();
            }
            ++it;
            if (it->first == "password")
            {
                user.password = it->second.get_value<std::string>();
            }

            Poco::MongoDB::Connection connection(MongoConfig::host, MongoConfig::port);
            MongoConnect::sendAuth(connection);

            if (MongoConnect::identification(user.username, connection) == true)
                throw Poco::InvalidArgumentException("User with this login already exists");

            user.hashPassword = Auth::sha256(user);
            MongoConnect::addNewUser(user, connection);

            throw Poco::SignalException("Sign up success");
        }
        else
        {
            if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS)
                response.send();
            else
                // error405send(request, response);
        }
    }
    catch (Poco::SignalException &ex)
    {
        // complete204send(request, response, ex.message());
    }
    catch (const Poco::InvalidArgumentException &ex)
    {
        // error400send(request, response, ex.message());
    }
    catch (const Poco::Net::NotAuthenticatedException &ex)
    {
        // error401send(request, response);
    }
    catch (const Poco::NotFoundException &ex)
    {
        // error404send(request, response);
    }
    catch (const Poco::ApplicationException &ex)
    {
        // error500send(request, response, ex.message());
    }
    catch (const Poco::Net::ConnectionRefusedException &ex)
    {
        // error502send(request, response, ex.message());
    }
}