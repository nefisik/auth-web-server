#include "database/mongodb/include/mongo.h"
#include "database/mongodb/include/configsdb.h"
#include "database/mongodb/include/mongodata.h"

// INSERT INTO users(username, hashPassword) VALUES(..., ...)
void MongoConnect::addNewUser(const User& user)
{
	Poco::MongoDB::Connection connection(MongoConfig::host, MongoConfig::port);

	std::cout << "*** addNewUser ***" << std::endl;

	Poco::MongoDB::Database db(MongoData::DbName);
	Poco::SharedPtr<Poco::MongoDB::InsertRequest> insertPlayerRequest = db.createInsertRequest(MongoData::CollectionName);
	// With one insert request, we can add multiple documents
	insertPlayerRequest->addNewDocument()
		.add(MongoData::username, user.username)
		.add(MongoData::hashPassword, user.password)
		.add(MongoData::token, user.token);

	connection.sendRequest(*insertPlayerRequest);
	std::string lastError = db.getLastError(connection);
	if (!lastError.empty())
	{
		std::cout << "Last Error: " << db.getLastError(connection) << std::endl;
	}
	else
	{
		std::cout << "ok" << std::endl;
	}
}

// SELECT username WHERE username = "..."
int MongoConnect::authentication(const User& user)
{
	Poco::MongoDB::Connection connection(MongoConfig::host, MongoConfig::port);

	std::cout << "*** authentication ***" << std::endl;
	int count = 0;

	Poco::MongoDB::Cursor cursor(MongoData::DbName, MongoData::CollectionName);
	// Selecting fields is done by adding them to the returnFieldSelector
	// Use 1 as value of the element.
	cursor.query().selector().add(MongoData::username, user.username);
	Poco::MongoDB::ResponseMessage& response = cursor.next(connection);
	for (;;)
	{
		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			std::cout << "ok" << std::endl;
			++count;
		}
		// When the cursorID is 0, there are no documents left, so break out ...
		if (response.cursorID() == 0)
		{
			break;
		}
		// Get the next bunch of documents
		response = cursor.next(connection);
	}
	return count;
}

// SELECT hashPassword WHERE username = "..."
std::string MongoConnect::getUserHashPassword(const User& user)
{
	Poco::MongoDB::Connection connection(MongoConfig::host, MongoConfig::port);

	std::cout << "*** getUserHashPassword ***" << std::endl;
	std::string password;
	Poco::MongoDB::Cursor cursor(MongoData::DbName, MongoData::CollectionName);
	// Selecting fields is done by adding them to the returnFieldSelector
	// Use 1 as value of the element.
	cursor.query().selector().add(MongoData::username, user.username);
	Poco::MongoDB::ResponseMessage& response = cursor.next(connection);
	for (;;)
	{
		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			password = (*it)->get<std::string>(MongoData::hashPassword);
		}
		// When the cursorID is 0, there are no documents left, so break out ...
		if (response.cursorID() == 0)
		{
			break;
		}
		// Get the next bunch of documents
		response = cursor.next(connection);
	}
	return password;
}

// SELECT hashPassword WHERE username = "..."
std::string MongoConnect::getUserToken(const User& user)
{
	Poco::MongoDB::Connection connection(MongoConfig::host, MongoConfig::port);

	std::cout << "*** getUserToken ***" << std::endl;
	std::string token;
	Poco::MongoDB::Cursor cursor(MongoData::DbName, MongoData::CollectionName);
	// Selecting fields is done by adding them to the returnFieldSelector
	// Use 1 as value of the element.
	cursor.query().selector().add(MongoData::username, user.username);
	Poco::MongoDB::ResponseMessage& response = cursor.next(connection);
	for (;;)
	{
		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			token = (*it)->get<std::string>(MongoData::token);
		}
		// When the cursorID is 0, there are no documents left, so break out ...
		if (response.cursorID() == 0)
		{
			break;
		}
		// Get the next bunch of documents
		response = cursor.next(connection);
	}
	return token;
}

// UPDATE UserDb SET hashPassword = "..." WHERE username = "..."
void MongoConnect::updateUserHashPassword(const User& user)
{
	Poco::MongoDB::Connection connection(MongoConfig::host, MongoConfig::port);

	std::cout << "*** updateUserHashPassword ***" << std::endl;

	Poco::MongoDB::Database db(MongoData::DbName);
	Poco::SharedPtr<Poco::MongoDB::UpdateRequest> request = db.createUpdateRequest(MongoData::CollectionName);
	request->selector().add(MongoData::username, user.username);
	request->update().add(MongoData::hashPassword, user.password);
	request->update().add(MongoData::token, user.token);
	connection.sendRequest(*request);
	Poco::MongoDB::Document::Ptr lastError = db.getLastErrorDoc(connection);
	// std::cout << "LastError: " << lastError->toString(2) << std::endl;
	if (!lastError.isNull())
	{
		std::cout << "Last Error: " << lastError->toString(2) << std::endl;
	}
	else
	{
		std::cout << "ok" << std::endl;
	}
}

// UPDATE UserDb SET token = "..." WHERE username = "..."
void MongoConnect::updateUserToken(const User& user)
{
	Poco::MongoDB::Connection connection(MongoConfig::host, MongoConfig::port);

	std::cout << "*** updateUserToken ***" << std::endl;

	Poco::MongoDB::Database db(MongoData::DbName);
	Poco::SharedPtr<Poco::MongoDB::UpdateRequest> request = db.createUpdateRequest(MongoData::CollectionName);
	request->selector().add(MongoData::username, user.username);
	request->update().add(MongoData::hashPassword, user.password);
	request->update().add(MongoData::token, user.token);
	connection.sendRequest(*request);
	Poco::MongoDB::Document::Ptr lastError = db.getLastErrorDoc(connection);
	// std::cout << "LastError: " << lastError->toString(2) << std::endl;
	if (!lastError.isNull())
	{
		std::cout << "Last Error: " << lastError->toString(2) << std::endl;
	}
	else
	{
		std::cout << "ok" << std::endl;
	}
}


// DELETE FROM UsersDb WHERE username = "..."
void MongoConnect::deleteUser(const User& user)
{
	Poco::MongoDB::Connection connection(MongoConfig::host, MongoConfig::port);

	std::cout << "*** deleteUser ***" << std::endl;

	Poco::MongoDB::Database db("UsersDb");
	Poco::SharedPtr<Poco::MongoDB::DeleteRequest> request = db.createDeleteRequest("users");
	request->selector().add("username", "kalikov");
	connection.sendRequest(*request);
	Poco::MongoDB::Document::Ptr lastError = db.getLastErrorDoc(connection);
	std::cout << "LastError: " << lastError->toString(2) << std::endl;
}
