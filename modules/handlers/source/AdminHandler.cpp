#include "handlers/include/AdminHandler.hpp"

AdminHandler::AdminHandler(const AdminCommands& command)
    : command(command) {}

AdminHandler::~AdminHandler() = default;

void AdminHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    response.set("Access-Control-Allow-Origin", CORS);
    response.set("Access-Control-Allow-Method", "GET, POST");

    try
    {
        authorizationAdmin(request, response);

        if ((request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET) ||
            (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST))
        {
            std::unique_ptr<AdminMethods> method(new AdminMethods());
            std::string resp_string;

            auto json = getJson(request);

            switch (command)
            {
                case AdminCommands::UPDATE_DATA:
                {
                    method->updateData(
                        getField(json, FrontData::username),
                        getField(json, FrontData::field),
                        getField(json, FrontData::newData));

                    break;
                }
                case AdminCommands::GET_ALL_USERS:
                {
                    resp_string = method->getAllUsers();
                    break;
                }
                case AdminCommands::GET_ALL_UNVERIFIED_USERS:
                {
                    resp_string = method->getAllUnverifiedUsers();
                    break;
                }
                case AdminCommands::GET_ALL_UNVERIFIED_MAIL_USERS:
                {
                    resp_string = method->getAllUnverifiedMailUsers();
                    break;
                }
                case AdminCommands::GET_ALL_ADMINS:
                {
                    resp_string = method->getAllAdmins();
                    break;
                }
                case AdminCommands::SEARCH_USER:
                {
                    resp_string = method->searchUser(
                        getField(json, FrontData::username));

                    break;
                }
                case AdminCommands::ADD_USER:
                {
                    method->addUser(
                        getField(json, FrontData::username),
                        getField(json, FrontData::password),
                        getField(json, FrontData::mail));

                    break;
                }
                case AdminCommands::ADD_ADMIN:
                {
                    method->addAdmin(
                        getField(json, FrontData::username),
                        getField(json, FrontData::password),
                        getField(json, FrontData::mail));

                    break;
                }
                case AdminCommands::DELETE_USER:
                {
                    method->deleteUser(
                        getField(json, FrontData::username));

                    break;
                }

                default:
                    break;
            }

            if(!resp_string.empty())
            {
                response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                Poco::Net::MediaType type("application/json");
                response.setContentType(type);
                std::ostream& out = response.send();
                out << resp_string;
            }
            else
            {
                response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NO_CONTENT);
                response.send();
            }

            printLogs(request, response);
        }
        else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS)
        {
            response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
            response.set("Access-Control-Allow-Headers", "token, Content-Type Accept");
            response.set("Access-Control-Request-Headers", "token, Content-Type Accept");
            response.send();

            printLogs(request, response);
        }
        else
        {
            throw Poco::Net::HTTPException("HTTP_METHOD_NOT_ALLOWED");
        }
    }
    catch (const Poco::InvalidArgumentException &ex)
    {
        const int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND;
        sendResponse(request, response, status, ex.message());
    }
    catch (const Poco::Net::NotAuthenticatedException &ex)
    {
        const int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_UNAUTHORIZED;
        sendResponse(request, response, status, ex.message());
    }
    catch (const Poco::InvalidAccessException &ex)
    {
        const int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_FORBIDDEN;
        sendResponse(request, response, status, ex.message());
    }
    catch (const Poco::NotFoundException &ex)
    {
        const int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND;
        sendResponse(request, response, status, ex.message());
    }
    catch (const Poco::ApplicationException &ex)
    {
        const int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_INTERNAL_SERVER_ERROR;
        sendResponse(request, response, status, ex.message());
    }
    catch (const Poco::Net::ConnectionRefusedException &ex)
    {
        const int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_BAD_GATEWAY;
        sendResponse(request, response, status, ex.message());
    }
    catch (const Poco::Redis::RedisException &ex)
    {
        const int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_INTERNAL_SERVER_ERROR;
        sendResponse(request, response, status, ex.message());
    }
    catch (const Poco::Net::HTTPException &ex)
    {
        const int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_METHOD_NOT_ALLOWED;
        sendResponse(request, response, status, ex.message());
    }
    catch (...)
    {
        const int status = Poco::Net::HTTPResponse::HTTPStatus::HTTP_INTERNAL_SERVER_ERROR;
        sendResponse(request, response, status, "HTTP_INTERNAL_SERVER_ERROR");
    }
}

boost::property_tree::ptree AdminHandler::getJson(Poco::Net::HTTPServerRequest& request)
{
    boost::property_tree::ptree json;
    std::string data;
    auto& stream = request.stream();
    getline(stream, data);
    std::stringstream ss;
    ss << data;
    if (!data.empty())
    {
        boost::property_tree::read_json(ss, json);
    }

    return json;
}

std::string AdminHandler::getField(boost::property_tree::ptree& json, const std::string& field)
{
    std::string data;

    for (const auto &it : json)
    {
        if (it.first == field)
        {
            data = it.second.get_value<std::string>();

            if (data.empty())
            {
                throw Poco::InvalidArgumentException("Поле '" + field + "' не заполнено");
            }
        }
    }

    if (data.empty())
    {
        throw Poco::InvalidArgumentException("'" + field + "' field is missing");
    }

    return data;
}