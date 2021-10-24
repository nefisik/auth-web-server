#pragma once

#include "preloader.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Util/ServerApplication.h"
#include <iostream>

class WebServerApp : public Poco::Util::ServerApplication
{
	void initialize(Poco::Util::Application &self);

	int main(const std::vector<std::string> &);
};