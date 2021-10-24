#include "handlers/include/SignOutRequestHandler.h"

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

            // printLogs(request, response);

            throw Poco::SignalException("Logout success");

        }
        else
        {
            if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS) {
                response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                response.set("Access-Control-Allow-Method", "GET, POST");
                response.set("Access-Control-Allow-Headers", "token, Content-Type, Accept");
                response.send();

                // printLogs(request, response);
            }
            // else
                // error405send(request, response);
        }
    }
    catch (const Poco::SignalException &ex)
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
    catch (const Poco::Redis::RedisException &ex)
    {
        // error502send(request, response, ex.message());
    }
}
