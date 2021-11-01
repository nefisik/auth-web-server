#include "server-run/include/preloader.hpp"
#include "handlers/include/Base/BaseHandler.hpp"

std::string getConfigValue(const std::string &val_name)
{
	auto configs = boost::property_tree::ptree();
	boost::property_tree::read_json("../../configs.json", configs);
	auto s = std::string(configs.get<std::string>(val_name));

	std::cout << "\nGETTING\t" + val_name + " --> COMPLETE \n";

	return s;
}

void checkMongoDbConnection()
{
    Poco::MongoDB::Connection connection(MongoConfig::host, MongoConfig::port);
    MongoConnect::sendAuth(connection, MongoConfig::password);

    std::cout << "\ncheck MongoDB Connection --> COMPLETE \n";  
}

void checkRedisDbConnection()
{
    Poco::Redis::Client redis;
	redis.connect(RedisConfig::host, RedisConfig::port);
    Redis::sendAuth(redis, RedisConfig::password);

    std::cout << "\ncheck RedisDB Connection --> COMPLETE \n\n";
}

void starter()
{
    checkMongoDbConnection();
    checkRedisDbConnection();

    boost::property_tree::ptree configs;
    boost::property_tree::read_json("../../configs.json", configs);

    try
	{
        std::cout << std::endl << "--- Running Server with params ---" << std::endl;
        for (const auto &x : configs)
			std::cout << x.first << " <---> " << x.second.data() << std::endl;
        std::cout << "--- LOGS THIS ---" << std::endl;
    }
	catch (Poco::Exception &e)
	{
        std::cerr << "Exception <--->" << e.message() << std::endl;
    }
	catch(std::exception& e)
	{
        std::cerr << "Exception <--->" << e.what() << std::endl;
    }
}
