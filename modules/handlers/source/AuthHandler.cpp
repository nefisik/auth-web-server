#include "handlers/include/AuthHandler.hpp"

AuthHandler::AuthHandler(const AuthCommands& command)
    : command(command) {}

AuthHandler::~AuthHandler() = default;

void AuthHandler::handleRequest(Request& request, Response& response)
{
    response.set("Access-Control-Allow-Origin", CORS);
    response.set("Access-Control-Allow-Method", "GET, POST");

    try
    {
        if ((request.getMethod() == HttpRequest::HTTP_GET) ||
            (request.getMethod() == HttpRequest::HTTP_POST))
        {
            std::string respString;
            auto json = getJson(request);

            switch (command)
            {
                case AuthCommands::SIGN_UP:
                {
                    method.signUp(
                        getField(json, FrontData::username),
                        getField(json, FrontData::password),
                        getField(json, FrontData::mail));

                    break;
                }
                case AuthCommands::SIGN_UP_VERIFY_URL:
                {
                    method.signUpVerify(
                        getUrlData(request.getURI(), SignUpVerifyURL));

                    break;
                }
                case AuthCommands::SIGN_IN:
                {
                    respString = method.signIn(
                        getField(json, FrontData::username),
                        getField(json, FrontData::password));

                    break;
                }
                case AuthCommands::SIGN_OUT:
                {
                    method.signOut(
                        getHeader(request, FrontData::token));

                    break;
                }
                case AuthCommands::REFRESH:
                {
                    respString = method.refresh(
                        getHeader(request, FrontData::token));

                    break;
                }
                case AuthCommands::MAIL_PASSWORD_RECOVERY:
                {
                    method.mailPasswordRecovery(
                        getField(json, FrontData::mail));

                    break;
                }
                case AuthCommands::CHECK_RECOVERY_TOKEN:
                {
                    respString = method.checkRecoveryToken(
                        getUrlData(request.getURI(), CheckRecoveryTokenURL));

                    break;
                }
                case AuthCommands::PASSWORD_RECOVERY:
                {
                    method.passwordRecovery(
                        getUsernameFromToken(request),
                        getField(json, FrontData::username),
                        getField(json, FrontData::password));

                    break;
                }
                default:
                    break;
            }

            sendResponse(request, response, respString);
        }
        else if (request.getMethod() == HttpRequest::HTTP_OPTIONS)
        {
            response.setStatus(Status::HTTP_OK);
            response.set("Access-Control-Allow-Headers", "token, Content-Type Accept");
            response.set("Access-Control-Request-Headers", "token, Content-Type Accept");
            response.send();

            printLogs(request, response);
        }
        else
        {
            throw Poco::Exception("HTTP_METHOD_NOT_ALLOWED", 405);
        }
    }
    catch (const Poco::Exception& ex)
    {
        sendError(request, response, ex.code(), ex.message());
    }
    catch (...)
    {
        sendError(request, response, 500, boost::current_exception_diagnostic_information());
    }
}