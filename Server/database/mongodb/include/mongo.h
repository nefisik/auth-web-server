#pragma once

#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/Cursor.h"
#include "Poco/MongoDB/Array.h"
#include "database/mongodb/include/user.h"

class MongoConnect
{
public:
	void addNewUser(const User &);
	bool authentication(const User &);
	std::string getUserHashPassword(const User &);
	std::string getUserToken(const User &);
	std::string getUserTokenToken(std::string);
	void updateUserUsername(const User &);
	void updateUserHashPassword(const User &);
	void updateUserToken(const User &);
	void logout(const std::string);
	void deleteUser(const User &);
};