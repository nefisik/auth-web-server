#include "server/include/preloader.hpp"
#include "server/include/Base/BaseHandler.hpp"

#include "database/mongodb/include/mongo.hpp"
#include "database/redis/include/redis.hpp"
#include "database/mail/include/mail.hpp"

std::string Preloader::getConfigValue(const std::string& val_name)
{
	auto config = boost::property_tree::ptree();
	boost::property_tree::read_json("../../config.json", config);
	const auto s = std::string(config.get<std::string>(val_name));

	std::cout << "\nGETTING\t" + val_name + " --> COMPLETE\n";

	return s;
}

void Preloader::checkMongoDbConnection()
{
    Mongo mongo;

    std::cout << "\ncheck MongoDB Connection --> COMPLETE\n";  
}

void Preloader::checkRedisDbConnection()
{
    Redis redis;

    std::cout << "\ncheck RedisDB Connection --> COMPLETE\n";
}

void Preloader::checkGMailConnection()
{
    Mail mail;

    std::cout << "\ncheck GMail Connection --> COMPLETE\n\n";
}

void Preloader::starter()
{
    checkMongoDbConnection();
    checkRedisDbConnection();
    checkGMailConnection();

    boost::property_tree::ptree config;
    boost::property_tree::read_json("../../config.json", config);

    try
	{
        std::cout << std::endl << "--- Running Server with params ---" << std::endl;
        for (const auto &x : config)
        {
            std::cout << x.first << " <---> " << x.second.data() << std::endl;
        }

        std::cout << "--- LOGS THIS ---" << std::endl;
    }
	catch (const Poco::Exception &ex)
	{
        std::cerr << "Exception <--->" << ex.message() << std::endl;
    }
	catch(const std::exception& ex)
	{
        std::cerr << "Exception <--->" << ex.what() << std::endl;
    }
}
