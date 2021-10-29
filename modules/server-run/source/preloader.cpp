#include "server-run/include/preloader.hpp"
#include "handlers/include/Base/BaseHandler.hpp"

std::string getHashSum(const std::string &fileContent)
{
	Poco::MD5Engine md5;
	Poco::DigestOutputStream ds(md5);
	ds << fileContent;
	ds.close();
	return Poco::DigestEngine::digestToHex(md5.digest());
}

void WriteOrRewriteConfig()
{
	if (!boost::filesystem::exists("../../configs.json"))
		throw Poco::ApplicationException("Not found configs.json in 'Web-Server' ");

	std::cout << "\nStarting Rewriting configs.json\n";

	std::ifstream in2("../../configs.json");
	std::ofstream out2("../../configs.json");

	char c1 = in2.get();

	while (in2.good())
	{
		out2 << c1;
		c1 = in2.get();
	}
	out2.close();
	in2.close();

	std::cout << "\nRewriting configs.json --> COMPLETE \n";
}

void mayBeNotWriteConfig()
{
	try
	{
		if (boost::filesystem::exists("../../configs.json"))
		{

			if (!boost::filesystem::exists("../../configs.json"))
				throw Poco::ApplicationException("Not found configs.json in 'Web-Server' ");

			auto configs_api = boost::property_tree::ptree();
			auto configs_root = boost::property_tree::ptree();
			boost::property_tree::read_json("../../configs.json", configs_api);
			boost::property_tree::read_json("../../configs.json", configs_root);

			auto buf1 = std::ostringstream();
			boost::property_tree::write_json(buf1, configs_api, false);
			auto buf2 = std::ostringstream();
			boost::property_tree::write_json(buf2, configs_root, false);

			auto hash1 = std::string(getHashSum(buf1.str()));
			auto hash2 = std::string(getHashSum(buf2.str()));

			if (hash1 != hash2)
				WriteOrRewriteConfig();
		}
		else
			WriteOrRewriteConfig();
	}
	catch (Poco::ApplicationException &ex)
	{
		std::cout << ex.what() << std::endl;
	}
}

std::string getConfigValue(const std::string &val_name)
{
	mayBeNotWriteConfig();

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