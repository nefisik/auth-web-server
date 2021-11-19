#pragma once

#include <Poco/Net/HTTPServer.h>
#include <Poco/URI.h>
#include "server/include/Base/static/urls.hpp"
#include "server/include/Base/static/AuthStatic.hpp"
#include "server/include/Base/static/AdminStatic.hpp"
#include "handlers/include/AuthHandler.hpp"
#include "handlers/include/AdminHandler.hpp"
#include "server/include/Base/ErrorHandler.hpp"

//!
//! \brief The Factory class
//! Класс API обработчик urls
//!

class Factory : public Poco::Net::HTTPRequestHandlerFactory
{
    using Request = Poco::Net::HTTPServerRequest;
    using Status = Poco::Net::HTTPResponse::HTTPStatus;

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Request& request);

private:
    bool methodURI(const std::string& valid_uri, const std::string& now_uri);
};
