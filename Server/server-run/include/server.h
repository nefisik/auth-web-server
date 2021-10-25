#pragma once

#include "preloader.h"
#include "Poco/Util/ServerApplication.h"

class WebServerApp : public Poco::Util::ServerApplication
{
	void initialize(Poco::Util::Application &self);

	int main(const std::vector<std::string> &);
};