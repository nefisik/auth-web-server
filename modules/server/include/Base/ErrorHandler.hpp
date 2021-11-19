#pragma once

#include "server/include/Base/BaseHandler.hpp"

//!
//! \brief The ErrorHandler class
//! Класс API обработчик ошибок
//!

class ErrorHandler : public BaseHandler
{
	using Request = Poco::Net::HTTPServerRequest;
	using Response = Poco::Net::HTTPServerResponse;
    using Status = Poco::Net::HTTPResponse::HTTPStatus;

private:
    Status status;
public:
    ErrorHandler(const Status& status);
    ~ErrorHandler();
    void handleRequest(Request& request, Response& response);
};