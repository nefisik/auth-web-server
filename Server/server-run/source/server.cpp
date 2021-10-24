#include "../include/server.h"
#include "factory/include/factory.h"

void WebServerApp::initialize(Poco::Util::Application &self)
{
    loadConfiguration();
    ServerApplication::initialize(self);
}

int WebServerApp::main(const std::vector<std::string> &)
{
    Poco::UInt16 port = static_cast<Poco::UInt16>(config().getUInt("port", stoi(getConfigValue("Port"))));

    Poco::Net::HTTPServer srv(new HelloRequestHandlerFactory, port);
    srv.start();
    std::cout << "\n--------------------------------\n" << std::endl;
    logger().information("HTTP Server started on port %hu", port);
    waitForTerminationRequest();
    logger().information("Stopping HTTP Server...");
    srv.stop();

    return Application::EXIT_OK;
}
