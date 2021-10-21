#include "include/mongodb/mongo.h"

void MongoConnect::sendAuth(Poco::MongoDB::Connection& connection)
{
    Poco::MongoDB::Database db(MongoConfig::DbName);
    if(!db.authenticate(connection, MongoConfig::user, MongoConfig::password))
		throw Poco::Net::ConnectionRefusedException("MongoDB authentication failed");
}

// INSERT INTO users(username, hashPassword) VALUES(..., ...)
void MongoConnect::addNewUser(const User &user, Poco::MongoDB::Connection& connection)
{
	Poco::MongoDB::Database db(MongoConfig::DbName);
	Poco::SharedPtr<Poco::MongoDB::InsertRequest> insertPlayerRequest = db.createInsertRequest(MongoConfig::CollectionName);
	insertPlayerRequest->addNewDocument()
		.add(MongoData::username, user.username)
		.add(MongoData::hashPassword, user.hashPassword);
		// .add(MongoData::status, user.status);

	connection.sendRequest(*insertPlayerRequest);
	std::string lastError = db.getLastError(connection);
	if (!lastError.empty())
        throw Poco::ApplicationException(db.getLastError(connection));
}

// SELECT username WHERE username = "..."
bool MongoConnect::identification(const std::string &username, Poco::MongoDB::Connection& connection)
{
	int auth = 0;
	Poco::MongoDB::Cursor cursor(MongoConfig::DbName, MongoConfig::CollectionName);
	cursor.query().selector().add(MongoData::username, username);
	Poco::MongoDB::ResponseMessage &response = cursor.next(connection);
	for (;;)
	{
		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			// std::cout << (*it)->get<std::string>(MongoData::username) << std::endl;
			++auth;
		}
		if (response.cursorID() == 0)
		{
			break;
		}
		// Get the next bunch of documents
		response = cursor.next(connection);
	};
	if(auth == 0)
		return false;
	if(auth == 1)
		return true;
	if(auth > 1)
		throw Poco::InvalidArgumentException("User with this login already exists");

	return false;
}

// SELECT hashPassword WHERE username = "..."
std::string MongoConnect::getUserHashPassword(const std::string &username, Poco::MongoDB::Connection& connection)
{
	std::string hash_password;
	Poco::MongoDB::Cursor cursor(MongoConfig::DbName, MongoConfig::CollectionName);
	cursor.query().selector().add(MongoData::username, username);
	Poco::MongoDB::ResponseMessage &response = cursor.next(connection);
	for (;;)
	{
		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			hash_password = (*it)->get<std::string>(MongoData::hashPassword);
		}
		if (response.cursorID() == 0)
		{
			break;
		}
		response = cursor.next(connection);
	};
	if (hash_password.empty())
		throw Poco::Net::NotAuthenticatedException("Failed get user hash password");

	return hash_password;
}

/**************************************BACKLOG**************************************/

// // UPDATE users SET username = "..." WHERE username = "..."
// void MongoConnect::updateUserUsername(const std::string &username, Poco::MongoDB::Connection& connection)
// {
// 	Poco::MongoDB::Database db(MongoConfig::DbName);
// 	Poco::SharedPtr<Poco::MongoDB::UpdateRequest> request = db.createUpdateRequest(MongoConfig::CollectionName);
// 	request->selector().add(MongoData::username, username);
// 	request->update().addNewDocument("$set").add(MongoData::username, username);
// 	connection.sendRequest(*request);
// 	Poco::MongoDB::Document::Ptr lastError = db.getLastErrorDoc(connection);
// 	if (lastError->isType<std::string>("err"))
//         throw Poco::ApplicationException(lastError->toString(2));
// }

// // UPDATE users SET hashPassword = "..." WHERE username = "..."
// void MongoConnect::updateUserHashPassword(const User &user, Poco::MongoDB::Connection& connection)
// {
// 	Poco::MongoDB::Database db(MongoConfig::DbName);
// 	Poco::SharedPtr<Poco::MongoDB::UpdateRequest> request = db.createUpdateRequest(MongoConfig::CollectionName);
// 	request->selector().add(MongoData::username, user.username);
// 	request->update().addNewDocument("$set").add(MongoData::hashPassword, user.hashPassword);
// 	connection.sendRequest(*request);
// 	Poco::MongoDB::Document::Ptr lastError = db.getLastErrorDoc(connection);
// 		throw Poco::ApplicationException(lastError->toString(2));
// }

// // DELETE FROM UsersDb WHERE username = "..."
// void MongoConnect::deleteUser(const std::string &username, Poco::MongoDB::Connection& connection)
// {
// 	Poco::MongoDB::Database db(MongoConfig::DbName);
// 	Poco::SharedPtr<Poco::MongoDB::DeleteRequest> request = db.createDeleteRequest(MongoConfig::CollectionName);
// 	request->selector().add(MongoData::username, username);
// 	connection.sendRequest(*request);
// 	Poco::MongoDB::Document::Ptr lastError = db.getLastErrorDoc(connection);
// 	if (lastError->isType<std::string>("err"))
//         throw Poco::ApplicationException(lastError->toString(2));
// }

/*************************************DEPRECATED************************************/

// // UPDATE users SET accessToken = "NULL" WHERE username = "..."
// void MongoConnect::updateUserAccessToken(const User &user, Poco::MongoDB::Connection& connection)
// {
// 	Poco::MongoDB::Database db(MongoConfig::DbName);
// 	Poco::SharedPtr<Poco::MongoDB::UpdateRequest> request = db.createUpdateRequest(MongoConfig::CollectionName);
// 	request->selector().add(MongoData::username, user.username);
// 	request->update().addNewDocument("$set").add(MongoData::accessToken, user.accessToken);
// 	connection.sendRequest(*request);
// 	Poco::MongoDB::Document::Ptr lastError = db.getLastErrorDoc(connection);
// 	if (lastError->isType<std::string>("err"))
//     	throw Poco::ApplicationException(lastError->toString(2));
// }

// // UPDATE users SET accessToken = "..." WHERE refreshToken = "..."
// void MongoConnect::updateUserAccessToken(const std::string &refreshToken, const std::string &accessToken, Poco::MongoDB::Connection& connection)
// {
// 	Poco::MongoDB::Database db(MongoConfig::DbName);
// 	Poco::SharedPtr<Poco::MongoDB::UpdateRequest> request = db.createUpdateRequest(MongoConfig::CollectionName);
// 	request->selector().add(MongoData::refreshToken, refreshToken);
// 	request->update().addNewDocument("$set").add(MongoData::accessToken, accessToken);
// 	connection.sendRequest(*request);
// 	Poco::MongoDB::Document::Ptr lastError = db.getLastErrorDoc(connection);
// 	if (lastError->isType<std::string>("err"))
//     	throw Poco::ApplicationException(lastError->toString(2));
// }

// // UPDATE users SET refreshToken = "NULL" WHERE username = "..."
// void MongoConnect::updateUserRefreshToken(const User &user, Poco::MongoDB::Connection& connection)
// {
// 	Poco::MongoDB::Database db(MongoConfig::DbName);
// 	Poco::SharedPtr<Poco::MongoDB::UpdateRequest> request = db.createUpdateRequest(MongoConfig::CollectionName);
// 	request->selector().add(MongoData::username, user.username);
// 	request->update().addNewDocument("$set").add(MongoData::refreshToken, user.refreshToken);
// 	connection.sendRequest(*request);
// 	Poco::MongoDB::Document::Ptr lastError = db.getLastErrorDoc(connection);
// 	if (lastError->isType<std::string>("err"))
//         throw Poco::ApplicationException(lastError->toString(2));
// }

// // UPDATE users SET accessToken = "NULL" WHERE refreshToken = "..."
// void MongoConnect::logout(const std::string &refreshToken, Poco::MongoDB::Connection& connection)
// {
// 	Poco::MongoDB::Database db(MongoConfig::DbName);
// 	Poco::SharedPtr<Poco::MongoDB::UpdateRequest> request = db.createUpdateRequest(MongoConfig::CollectionName);
// 	request->selector().add(MongoData::refreshToken, refreshToken);
// 	request->update().addNewDocument("$set").add(MongoData::refreshToken, MongoData::tokenNULL);
// 	request->update().addNewDocument("$set").add(MongoData::accessToken, MongoData::tokenNULL);
// 	connection.sendRequest(*request);
// 	Poco::MongoDB::Document::Ptr lastError = db.getLastErrorDoc(connection);
// 	if (lastError->isType<std::string>("err"))
//         throw Poco::ApplicationException(lastError->toString(2));
// }

// // SELECT refreshToken WHERE username = "..."
// std::string MongoConnect::getUserRefreshToken(const std::string &username, Poco::MongoDB::Connection& connection)
// {
// 	std::string refresh_token;
// 	Poco::MongoDB::Cursor cursor(MongoConfig::DbName, MongoConfig::CollectionName);
// 	cursor.query().selector().add(MongoData::username, username);
// 	Poco::MongoDB::ResponseMessage &response = cursor.next(connection);
// 	for (;;)
// 	{
// 		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
// 		{
// 			refresh_token = (*it)->get<std::string>(MongoData::refreshToken);
// 		}
// 		if (response.cursorID() == 0)
// 		{
// 			break;
// 		}
// 		response = cursor.next(connection);
// 	};
// 	if (refresh_token.empty())
// 		throw Poco::Net::NotAuthenticatedException("Failed get user refresh token");

// 	return refresh_token;
// }

// // SELECT accessToken WHERE token = "..."
// std::string MongoConnect::getUserAccessToken(const std::string &refreshToken, Poco::MongoDB::Connection& connection)
// {
// 	std::string db_access_token;
// 	Poco::MongoDB::Cursor cursor(MongoConfig::DbName, MongoConfig::CollectionName);
// 	cursor.query().selector().add(MongoData::refreshToken, refreshToken);
// 	Poco::MongoDB::ResponseMessage &response = cursor.next(connection);
// 	for (;;)
// 	{
// 		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
// 		{
// 			db_access_token = (*it)->get<std::string>(MongoData::accessToken);
// 		}
// 		if (response.cursorID() == 0)
// 		{
// 			break;
// 		}
// 		// Get the next bunch of documents
// 		response = cursor.next(connection);
// 	};
// 	if (db_access_token.empty())
// 		throw Poco::Net::NotAuthenticatedException("Failed get user access token");

// 	return db_access_token;
// }

// // SELECT refreshToken WHERE refreshToken = "..."
// void MongoConnect::checkRefreshToken(const std::string &refreshToken, Poco::MongoDB::Connection &connection)
// {
// 	int auth = 0;
// 	Poco::MongoDB::Cursor cursor(MongoConfig::DbName, MongoConfig::CollectionName);
// 	cursor.query().selector().add(MongoData::refreshToken, refreshToken);
// 	Poco::MongoDB::ResponseMessage &response = cursor.next(connection);
// 	for (;;)
// 	{
// 		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
// 		{
// 			// std::cout << (*it)->get<std::string>(MongoData::username) << std::endl;
// 			++auth;
// 		}
// 		if (response.cursorID() == 0)
// 		{
// 			break;
// 		}
// 		// Get the next bunch of documents
// 		response = cursor.next(connection);
// 	}
// 	if (auth == 0)
//         throw Poco::Net::NotAuthenticatedException("Failed check user refresh token");
// }

// // SELECT accessToken WHERE accessToken = "..."
// void MongoConnect::checkAccessToken(const std::string &accessToken, Poco::MongoDB::Connection &connection)
// {
// 	int auth = 0;
// 	Poco::MongoDB::Cursor cursor(MongoConfig::DbName, MongoConfig::CollectionName);
// 	cursor.query().selector().add(MongoData::accessToken, accessToken);
// 	Poco::MongoDB::ResponseMessage &response = cursor.next(connection);
// 	for (;;)
// 	{
// 		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
// 		{
// 			// std::cout << (*it)->get<std::string>(MongoData::username) << std::endl;
// 			++auth;
// 		}
// 		if (response.cursorID() == 0)
// 		{
// 			break;
// 		}
// 		// Get the next bunch of documents
// 		response = cursor.next(connection);
// 	}
// 	if (auth == 0)
// 		throw Poco::Net::NotAuthenticatedException("Failed check user access token");
// }