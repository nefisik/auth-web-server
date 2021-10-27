#include "handlers/include/AuthRefreshTokenHandler.hpp"

void AuthRefreshTokenHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    request.set("Access-Control-Allow-Origin", CORS);
    response.set("Access-Control-Allow-Origin", CORS);

    try
    {
        if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
        {
            Poco::Redis::Client redis;
            redis.connect(RedisConfig::host, RedisConfig::port);
            Redis::sendAuth(redis, RedisConfig::password);

            auto refreshToken = request.get("token");
            if (!Auth::check_refresh_token(refreshToken))
                throw Poco::Net::NotAuthenticatedException("Unauthorized");

            auto accessToken = Redis::get(redis, refreshToken);
            if (Auth::check_access_token(accessToken))
            {
                accessToken = Auth::create_access_token(refreshToken);
                Redis::set(redis, refreshToken, accessToken);
                Redis::expire(redis, refreshToken, JWTparams::accessTokenLifetimeSeconds);
            }
            else
                Redis::del(redis, refreshToken);

            std::string access = accessToken;
            std::string msg = "Refresh token update success";
            int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK;

            sendResponseAccess(request, response, status, msg, access);
        }
        else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS)
        {
            response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
            response.set("Access-Control-Allow-Method", "GET, POST");
            response.set("Access-Control-Allow-Headers", "token");
            response.send();
        }
        else
            throw Poco::NotFoundException();
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
