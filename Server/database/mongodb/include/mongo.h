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
	void addNewUser(const User&);
	int authentication(const User&);
	std::string getUserHashPassword(const User&);
	std::string getUserToken(const User&);
	void updateUserHashPassword(const User&);
	void updateUserToken(const User&);
	void deleteUser(const User&);
};