#pragma once

#include <string>

struct User{
	int id;
	std::string username;
	std::string password;
	std::string token;
	bool tokenValid;
	bool passwordValid;
};