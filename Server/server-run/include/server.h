#pragma once

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Util/ServerApplication.h"
#include <iostream>

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/URI.h"

#include "Poco/Net/HTMLForm.h"

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;

class WebServerApp : public ServerApplication
{
	void initialize(Application &self);

	int main(const std::vector<std::string> &);
};