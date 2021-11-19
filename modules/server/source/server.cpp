#include "server/include/server.hpp"
#include "server/include/Base/Factory.hpp"

void WebServerApp::initialize(Poco::Util::Application &self)
{
    loadConfiguration();
    ServerApplication::initialize(self);
}

int WebServerApp::main(const std::vector<std::string> &)
{
    Poco::UInt16 port = static_cast<Poco::UInt16>(config().getUInt("port", stoi(Preloader::getConfigValue("Port"))));

    Poco::Net::HTTPServer srv(new Factory, port);
    srv.start();
    Preloader::starter();
    waitForTerminationRequest();
    logger().information("Stopping HTTP Server...");
    srv.stop();

    return Application::EXIT_OK;
}
