#include "handlers/include/SignUpRequestHandler.hpp"

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
            MongoConnect::sendAuth(connection, MongoConfig::password);

            if (MongoConnect::identification(user.username, connection) == true)
                throw Poco::InvalidArgumentException("User with this login already exists");

            user.hashPassword = Auth::sha256(user);
            MongoConnect::addNewUser(user, connection);

            int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK;
            std::string msg = "Sign up success";

            sendResponse(request, response, status, msg);
        }
        else
        {
            if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS)
                response.send();
            else
                throw Poco::NotFoundException();
        }
    }
    catch (const Poco::InvalidArgumentException &ex)
    {
        int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_BAD_REQUEST;
        sendResponse(request, response, status, ex.message());
    }
    catch (const Poco::Net::NotAuthenticatedException &ex)
    {
        int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_UNAUTHORIZED;
        sendResponse(request, response, status, ex.message());
    }
    catch (const Poco::NotFoundException &ex)
    {
        int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND;
        sendResponse(request, response, status, ex.message());
    }
    catch (const Poco::ApplicationException &ex)
    {
        int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_INTERNAL_SERVER_ERROR;
        sendResponse(request, response, status, ex.message());
    }
    catch (const Poco::Net::ConnectionRefusedException &ex)
    {
        int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_BAD_GATEWAY;
        sendResponse(request, response, status, ex.message());
    }
}