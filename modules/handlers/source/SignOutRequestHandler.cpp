#include "handlers/include/SignOutRequestHandler.hpp"

void SignOutRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    request.set("Access-Control-Allow-Origin", CORS);
    response.set("Access-Control-Allow-Origin", CORS);

    try
    {
        if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
        {
            auto refreshToken = request.get("token");

            Poco::Redis::Client redis;
            redis.connect(RedisConfig::host, RedisConfig::port);
            Redis::sendAuth(redis, RedisConfig::password);

            Redis::del(redis, refreshToken);

            int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK;
            std::string msg = "Sign out success";

            sendResponse(request, response, status, msg);
        }
        else
        {
            if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS) {
                response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                response.set("Access-Control-Allow-Method", "GET, POST");
                response.set("Access-Control-Allow-Headers", "token, Content-Type, Accept");
                response.send();

                printLogs(request, response);
            }
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
