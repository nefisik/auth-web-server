#include "handlers/include/AuthRefreshTokenHandler.hpp"

void AuthRefreshTokenHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    request.set("Access-Control-Allow-Origin", CORS);
    response.set("Access-Control-Allow-Origin", CORS);

    try
    {
        if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
        {
            Poco::MongoDB::Connection connection(MongoConfig::host, MongoConfig::port);
            Poco::Redis::Client redis;
            redis.connect(RedisConfig::host, RedisConfig::port);
            Redis::sendAuth(redis, RedisConfig::password);

            auto refreshToken = request.get("token");
            if (!Auth::check_refresh_token(refreshToken))
                throw Poco::Net::NotAuthenticatedException();

            auto accessToken = Redis::get(redis, refreshToken);
            if (Auth::check_access_token(accessToken))
            {
                accessToken = Auth::create_access_token(refreshToken);
                Redis::set(redis, refreshToken, accessToken);
                Redis::expire(redis, refreshToken, JWTparams::accessTokenLifetimeSeconds);
            }
            else
                Redis::del(redis, refreshToken);

            auto resp_data = accessToken;
            auto resp_status = "200";

            boost::property_tree::ptree resp;
            resp.put("STATUS", resp_status);
            resp.put("COMMENT", "OK");
            resp.put("ACCESS", resp_data);

            std::stringstream ss;
            boost::property_tree::json_parser::write_json(ss, resp);

            Poco::Net::MediaType type("application/json");
            response.setContentType(type);
            std::ostream &out = response.send();
            out << ss.str();
            out.flush();

            printLogs(request, response);
        }
        else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS)
        {
            response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
            response.set("Access-Control-Allow-Method", "GET, POST");
            response.set("Access-Control-Allow-Headers", "token");
            response.send();
        }
        // else
            // error405send(request, response);
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
