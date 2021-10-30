#include "handlers/include/AuthHandler.hpp"
#include "methods/include/AuthMethods.hpp"

AuthRequestHandler::AuthRequestHandler(const AuthCommands &command)
    :command(command) {}

AuthRequestHandler::~AuthRequestHandler() = default;

void AuthRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try
    {
        AuthMethods method;

        switch (command)
        {
            case AuthCommands::SIGN_UP:
            {
                response.set("Access-Control-Allow-Origin", CORS);
                response.set("Access-Control-Allow-Method", "POST");

                if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
                {
                    method.SIGN_UP(
                        getUserData(request));

                    int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK;
                    std::string msg = "Sign up success";
                    sendResponse(request, response, status, msg);
                }
                else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS)
                    response.send();
                else
                    throw Poco::Net::HTTPException("Method not allowed");

                break;
            }
            case AuthCommands::SIGN_IN:
            {
                response.set("Access-Control-Allow-Origin", CORS);
                response.set("Access-Control-Allow-Method", "POST");
                response.set("Access-Control-Allow-Headers", "token");

                if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
                {
                    auto tokens = method.SIGN_IN(
                        getUserData(request));

                    std::string refreshToken = tokens.first;
                    std::string accessToken = tokens.second;
                    int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK;
                    std::string msg = "OK";
                    sendResponseTokens(request, response, status, msg, refreshToken, accessToken);
                }
                else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS)
                    response.send();
                else
                    throw Poco::Net::HTTPException("Method not allowed");

                break;
            }
            case AuthCommands::SIGN_OUT:
            {
                response.set("Access-Control-Allow-Origin", CORS);

                if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
                {
                    method.SIGN_OUT(
                        getToken(request));

                    int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK;
                    std::string msg = "Sign out success";
                    sendResponse(request, response, status, msg);
                }
                else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS)
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                    response.set("Access-Control-Allow-Method", "GET, POST");
                    response.set("Access-Control-Allow-Headers", "token, Content-Type, Accept");
                    response.send();

                    printLogs(request, response);
                }
                else
                    throw Poco::Net::HTTPException("Method not allowed");

                break;
            }
            case AuthCommands::REFRESH:
            {
                response.set("Access-Control-Allow-Origin", CORS);

                if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
                {
                    auto accessToken = method.REFRESH(
                        getToken(request));

                    std::string msg = "Refresh token update success";
                    int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK;
                    sendResponseAccess(request, response, status, msg, accessToken);
                }
                else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS)
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                    response.set("Access-Control-Allow-Method", "GET, POST");
                    response.set("Access-Control-Allow-Headers", "token");
                    response.send();
                }
                else
                    throw Poco::Net::HTTPException("Method not allowed");

                break;
            }
            default:
                break;
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
    catch(const Poco::Net::HTTPException &ex)
    {
        int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_METHOD_NOT_ALLOWED;
        sendResponse(request, response, status, ex.message());
    }
    catch (...)
    {
        int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_UNAUTHORIZED;
        sendResponse(request, response, status, "Unauthorized");
    }
}

std::string AuthRequestHandler::getToken(Poco::Net::HTTPServerRequest &request)
{
    if(!request.has("token"))
        throw Poco::InvalidArgumentException("'token' field is missing");

    std::string refreshToken = request.get("token");

    return refreshToken;
}

User AuthRequestHandler::getUserData(Poco::Net::HTTPServerRequest &request)
{
    User user;

    boost::property_tree::ptree pt;
    boost::property_tree::read_json(request.stream(), pt);
    boost::property_tree::ptree::const_iterator it = pt.begin();
    if (it->first == "username")
    {
        user.username = it->second.get_value<std::string>();
    }
    else
        throw Poco::InvalidArgumentException("'username' field is missing");
    
    ++it;
    
    if (it->first == "password")
    {
        user.password = it->second.get_value<std::string>();
    }
    else
        throw Poco::InvalidArgumentException("'password' field is missing");

    return user;
}
