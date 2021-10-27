#include "handlers/include/SignInRequestHandler.hpp"

void SignInRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    request.set("Access-Control-Allow-Origin", CORS);
    response.set("Access-Control-Allow-Origin", CORS);
    response.set("Access-Control-Allow-Method", "POST");
    response.set("Access-Control-Allow-Headers", "token");

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
            Poco::Redis::Client redis;
            redis.connect(RedisConfig::host, RedisConfig::port);
            Redis::sendAuth(redis, RedisConfig::password);

            if (MongoConnect::identification(user.username, connection) == false)
                throw Poco::Net::NotAuthenticatedException("User does not exist");

            auto userHashPassword = MongoConnect::getUserHashPassword(user.username, connection);
            user.hashPassword = Auth::sha256(user);

            if (userHashPassword == user.hashPassword)
            {
                user.refreshToken = Auth::create_refresh_token(user);
                user.accessToken = Auth::create_access_token(user);

                Redis::set(redis, user.refreshToken, user.accessToken);
                Redis::expire(redis, user.refreshToken, JWTparams::accessTokenLifetimeSeconds);

                std::string refresh = user.refreshToken;
                std::string access = user.accessToken;
                int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK;
                std::string msg = "OK";

                sendResponseTokens(request, response, status, msg, refresh, access);
            }
            else
                throw Poco::Net::NotAuthenticatedException("Unauthorized");
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
    catch (const Poco::Redis::RedisException &ex)
    {
        int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_BAD_GATEWAY;
        sendResponse(request, response, status, ex.message());
    }
}
