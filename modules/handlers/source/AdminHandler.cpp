#include "handlers/include/AdminHandler.hpp"

AdminHandler::AdminHandler(const AdminCommands& command)
    : command(command) {}

AdminHandler::~AdminHandler() = default;

void AdminHandler::handleRequest(Request& request, Response& response)
{
    response.set("Access-Control-Allow-Origin", CORS);
    response.set("Access-Control-Allow-Method", "GET, POST");

    try
    {
        authorizationAdmin(request, response);

        if ((request.getMethod() == HttpRequest::HTTP_GET) ||
            (request.getMethod() == HttpRequest::HTTP_POST))
        {
            std::string respString;
            auto json = getJson(request);

            switch (command)
            {
                case AdminCommands::UPDATE_DATA:
                {
                    method.updateData(
                        getUsernameFromToken(request),
                        getField(json, FrontData::username),
                        getField(json, FrontData::field),
                        getField(json, FrontData::newData));

                    break;
                }
                case AdminCommands::GET_ALL_USERS:
                {
                    respString = method.getAllUsers();
                    break;
                }
                case AdminCommands::GET_ALL_UNVERIFIED_USERS:
                {
                    respString = method.getAllUnverifiedUsers();
                    break;
                }
                case AdminCommands::GET_ALL_UNVERIFIED_MAIL_USERS:
                {
                    respString = method.getAllUnverifiedMailUsers();
                    break;
                }
                case AdminCommands::GET_ALL_ADMINS:
                {
                    respString = method.getAllAdmins();
                    break;
                }
                case AdminCommands::SEARCH_USER:
                {
                    respString = method.searchUser(
                        getField(json, FrontData::username));

                    break;
                }
                case AdminCommands::ADD_USER:
                {
                    method.addUser(
                        getField(json, FrontData::username),
                        getField(json, FrontData::password),
                        getField(json, FrontData::mail));

                    break;
                }
                case AdminCommands::ADD_ADMIN:
                {
                    method.addAdmin(
                        getField(json, FrontData::username),
                        getField(json, FrontData::password),
                        getField(json, FrontData::mail));

                    break;
                }
                case AdminCommands::DELETE_USER:
                {
                    method.deleteUser(
                        getField(json, FrontData::username));

                    break;
                }

                default:
                    break;
            }

            sendResponse(request, response, respString);
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