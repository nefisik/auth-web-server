#include "include/handlers/SignInRequestHandler.h"

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

            std::string resp_data1 = "";
            std::string resp_data2 = "";
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
            auto redis = Poco::Redis::Client(RedisConfig::host, RedisConfig::port);
            Redis::sendAuth(redis, RedisConfig::password);

            if (MongoConnect::identification(user.username, connection) == false)
                throw Poco::Net::NotAuthenticatedException("Failed user identifacation");

            auto userHashPassword = MongoConnect::getUserHashPassword(user.username, connection);
            user.hashPassword = Auth::sha256(user);

            if (userHashPassword == user.hashPassword)
            {
                user.refreshToken = Auth::create_refresh_token(user);
                user.accessToken = Auth::create_access_token(user);

                Redis::set(redis, user.refreshToken, user.accessToken);
                Redis::expire(redis, user.refreshToken, JWTparams::accessTokenLifetimeSeconds);

                response.setStatus(Poco::Net::HTTPServerResponse::HTTP_OK);
                resp_status = "200";
                resp_data1 = user.refreshToken;
                resp_data2 = user.accessToken;
            }
            else
                throw Poco::Net::NotAuthenticatedException();

            boost::property_tree::ptree resp;
            resp.put("STATUS", resp_status);
            resp.put("COMMENT", "OK");
            resp.put("REFRESH", resp_data1);
            resp.put("ACCESS", resp_data2);

            std::stringstream ss;
            boost::property_tree::json_parser::write_json(ss, resp);

            Poco::Net::MediaType type("application/json");
            response.setContentType(type);
            std::ostream &out = response.send();
            out << ss.str();
            out.flush();

            printLogs(request, response);
        }
        else
        {
            if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS)
                response.send();
            else
                // error405send(request, response);
        }
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
    catch (const Poco::Redis::RedisException &ex)
    {
        // error502send(request, response, ex.message());
    }
}
