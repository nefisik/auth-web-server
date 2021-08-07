#pragma once

#include <string>

namespace PgConfig{
std::string host = "localhost";
	std::string port = "5432";
	std::string db = "auth";
	std::string user = "postgres";
	std::string password = "user";

	std::string connectionString = "host=" + host + " user=" + user + " password" + password + " dbname=" + db;
} //PgConfig