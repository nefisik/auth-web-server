#include "database/mongodb/include/mongo.hpp"

Mongo::Mongo()
{
    connection.connect(MongoConfig::host, MongoConfig::port);
    Mongo::sendAuth(MongoConfig::password);
}

void Mongo::sendAuth(const std::string& dbPassword)
{
    Poco::MongoDB::Database db(MongoConfig::DbAuthName);
    if(!db.authenticate(connection, MongoConfig::user, dbPassword))
	{
		throw Poco::Net::ConnectionRefusedException("MongoDB authentication failed");
	}
}

// INSERT INTO users(username, mail, hashPassword, status, verification, mailVerification) VALUES(..., ...)
void Mongo::addUser(const User& user)
{
	Poco::MongoDB::Database db(MongoConfig::DbName);
	Poco::SharedPtr<Poco::MongoDB::InsertRequest> insertPlayerRequest = db.createInsertRequest(MongoConfig::CollectionName);
	insertPlayerRequest->addNewDocument()
		.add(MongoData::username, user.username)
		.add(MongoData::mail, user.mail)
		.add(MongoData::hashPassword, user.hashPassword)
		.add(MongoData::status, MongoData::params::STATUS_USER)
		.add(MongoData::verification, MongoData::params::VERIFICATION_FALSE)
		.add(MongoData::mailVerification, MongoData::params::MAIL_VERIFICATION_FALSE);

	connection.sendRequest(*insertPlayerRequest);
	std::string lastError = db.getLastError(connection);
	if (!lastError.empty())
	{
		throw Poco::ApplicationException(db.getLastError(connection));
	}
}

// SELECT username WHERE username = "..."
bool Mongo::identification(const std::string& username)
{
	int auth = 0;
	Poco::MongoDB::Cursor cursor(MongoConfig::DbName, MongoConfig::CollectionName);
	cursor.query().selector().add(MongoData::username, username);
	Poco::MongoDB::ResponseMessage &response = cursor.next(connection);
	for (;;)
	{
		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			++auth;
		}
		if (response.cursorID() == 0)
		{
			break;
		}

		response = cursor.next(connection);
	}
	if(auth == 0)
	{
		return false;
	}

	return true;
}

void Mongo::checkMail(const std::string& mail, const std::string& username)
{
	int numMail = 0;
	Poco::MongoDB::Cursor cursor(MongoConfig::DbName, MongoConfig::CollectionName);
	cursor.query().selector().add(MongoData::mail, mail);
	Poco::MongoDB::ResponseMessage &response = cursor.next(connection);
	for (;;)
	{
		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			std::string user = (*it)->get<std::string>(MongoData::username);
			if (user != username)
			{
				++numMail;
			}
		}
		if (response.cursorID() == 0)
		{
			break;
		}

		response = cursor.next(connection);
	}
	if(numMail >= 1)
	{
		throw Poco::InvalidArgumentException("Пользователь с данной почтой уже существует");
	}
}

void Mongo::checkVerification(const std::string& username)
{
	std::string verif;
	Poco::MongoDB::Cursor cursor(MongoConfig::DbName, MongoConfig::CollectionName);
	cursor.query().selector().add(MongoData::username, username);
	Poco::MongoDB::ResponseMessage &response = cursor.next(connection);
	for (;;)
	{
		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			verif = (*it)->get<std::string>(MongoData::verification);
		}
		if (response.cursorID() == 0)
		{
			break;
		}
		response = cursor.next(connection);
	}
	if (verif.empty())
	{
		throw Poco::ApplicationException("Failed get user verification");
	}

	if(verif != MongoData::params::VERIFICATION_TRUE)
	{
		throw Poco::InvalidArgumentException("Вы неверифицированный пользователь.<br/>Дождитесь верификации администратором");
	}
}

void Mongo::checkMailVerification(const std::string& username)
{
	std::string mailVerif;
	std::string mail;
	Poco::MongoDB::Cursor cursor(MongoConfig::DbName, MongoConfig::CollectionName);
	cursor.query().selector().add(MongoData::username, username);
	Poco::MongoDB::ResponseMessage &response = cursor.next(connection);
	for (;;)
	{
		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			mailVerif = (*it)->get<std::string>(MongoData::mailVerification);
			mail = (*it)->get<std::string>(MongoData::mail);
		}
		if (response.cursorID() == 0)
		{
			break;
		}
		response = cursor.next(connection);
	}
	if (mailVerif.empty())
	{
		throw Poco::ApplicationException("Failed get user mail verification");
	}

	if(mailVerif != MongoData::params::MAIL_VERIFICATION_TRUE)
	{
		throw Poco::InvalidAccessException("Вы не подтвердили почту.<br/>Перейдите по ссылке, отправленной на почту " + mail);
	}
}

void Mongo::checkHashPassword(const std::string& username, const std::string& hashPassword)
{
	std::string db_hashPassword;
	Poco::MongoDB::Cursor cursor(MongoConfig::DbName, MongoConfig::CollectionName);
	cursor.query().selector().add(MongoData::username, username);
	Poco::MongoDB::ResponseMessage &response = cursor.next(connection);
	for (;;)
	{
		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			db_hashPassword = (*it)->get<std::string>(MongoData::hashPassword);
		}
		if (response.cursorID() == 0)
		{
			break;
		}
		response = cursor.next(connection);
	}
	if (db_hashPassword.empty())
	{
		throw Poco::ApplicationException("Failed get user hash password");
	}

	if(db_hashPassword != hashPassword)
	{
		throw Poco::InvalidArgumentException("Неверный пароль");
	}
}

// SELECT status WHERE username = "..."
std::string Mongo::getStatus(const std::string& username)
{
	std::string status;
	Poco::MongoDB::Cursor cursor(MongoConfig::DbName, MongoConfig::CollectionName);
	cursor.query().selector().add(MongoData::username, username);
	Poco::MongoDB::ResponseMessage &response = cursor.next(connection);
	for (;;)
	{
		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			status = (*it)->get<std::string>(MongoData::status);
		}
		if (response.cursorID() == 0)
		{
			break;
		}
		response = cursor.next(connection);
	}
	if (status.empty())
	{
		throw Poco::ApplicationException("Failed get user status");
	}

	return status;
}

// SELECT username WHERE mail = "..."
std::string Mongo::getUsername(const std::string& mail)
{
	std::string username;
	Poco::MongoDB::Cursor cursor(MongoConfig::DbName, MongoConfig::CollectionName);
	cursor.query().selector().add(MongoData::mail, mail);
	Poco::MongoDB::ResponseMessage &response = cursor.next(connection);
	for (;;)
	{
		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			username = (*it)->get<std::string>(MongoData::username);
		}
		if (response.cursorID() == 0)
		{
			break;
		}
		response = cursor.next(connection);
	}
	if (username.empty())
	{
		throw Poco::InvalidArgumentException("Неверная почта");
	}

	return username;
}

void Mongo::verifyMail(const std::string& username)
{
	Poco::MongoDB::Database db(MongoConfig::DbName);
	Poco::SharedPtr<Poco::MongoDB::UpdateRequest> request = db.createUpdateRequest(MongoConfig::CollectionName);
	request->selector().add(MongoData::username, username);
	request->update().addNewDocument("$set").add(MongoData::mailVerification, MongoData::params::MAIL_VERIFICATION_TRUE);
	connection.sendRequest(*request);
	Poco::MongoDB::Document::Ptr lastError = db.getLastErrorDoc(connection);
	if (lastError->isType<std::string>("err"))
	{
		throw Poco::ApplicationException(lastError->toString(2));
	}
}

/***************************************ADMIN**************************************/

// UPDATE users SET ... = "..." WHERE username = "..."
void Mongo::adminUpdateData(const UpdateData& data)
{
	Poco::MongoDB::Database db(MongoConfig::DbName);
	Poco::SharedPtr<Poco::MongoDB::UpdateRequest> request = db.createUpdateRequest(MongoConfig::CollectionName);
	request->selector().add(MongoData::username, data.username);
	request->update().addNewDocument("$set").add(data.field, data.newData);
	connection.sendRequest(*request);
	Poco::MongoDB::Document::Ptr lastError = db.getLastErrorDoc(connection);
	if (lastError->isType<std::string>("err"))
	{
		throw Poco::ApplicationException(lastError->toString(2));
	}
}

std::vector<User> Mongo::adminGetAllUsers()
{
	Poco::MongoDB::Cursor cursor(MongoConfig::DbName, MongoConfig::CollectionName);
	Poco::MongoDB::ResponseMessage& response = cursor.next(connection);

	std::vector<User> allUsers;
	User user;

	for (;;)
	{
		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			user.username = (*it)->get<std::string>(MongoData::username);
			user.mail = (*it)->get<std::string>(MongoData::mail);
			user.status = (*it)->get<std::string>(MongoData::status);
			user.verification = (*it)->get<std::string>(MongoData::verification);
			user.mailVerification = (*it)->get<std::string>(MongoData::mailVerification);

			allUsers.emplace_back(user);
		}

		if (response.cursorID() == 0)
		{
			break;
		}

		response = cursor.next(connection);
	}

	return allUsers;
}

std::vector<User> Mongo::adminGetAllUnverifiedUsers()
{
	Poco::MongoDB::Cursor cursor(MongoConfig::DbName, MongoConfig::CollectionName);
	cursor.query().selector().add(MongoData::verification, MongoData::params::VERIFICATION_FALSE);
	Poco::MongoDB::ResponseMessage &response = cursor.next(connection);

	std::vector<User> unverifiedUsers;
	User user;

	for (;;)
	{
		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			user.username = (*it)->get<std::string>(MongoData::username);
			user.mail = (*it)->get<std::string>(MongoData::mail);
			user.status = (*it)->get<std::string>(MongoData::status);
			user.verification = (*it)->get<std::string>(MongoData::verification);
			user.mailVerification = (*it)->get<std::string>(MongoData::mailVerification);

			unverifiedUsers.emplace_back(user);
		}

		if (response.cursorID() == 0)
		{
			break;
		}

		response = cursor.next(connection);
	}

	return unverifiedUsers;
}

std::vector<User> Mongo::adminGetAllUnverifiedMailUsers()
{
	Poco::MongoDB::Cursor cursor(MongoConfig::DbName, MongoConfig::CollectionName);
	cursor.query().selector().add(MongoData::verification, MongoData::params::MAIL_VERIFICATION_FALSE);
	Poco::MongoDB::ResponseMessage &response = cursor.next(connection);

	std::vector<User> unverifiedMailUsers;
	User user;

	for (;;)
	{
		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			user.username = (*it)->get<std::string>(MongoData::username);
			user.mail = (*it)->get<std::string>(MongoData::mail);
			user.status = (*it)->get<std::string>(MongoData::status);
			user.verification = (*it)->get<std::string>(MongoData::verification);
			user.mailVerification = (*it)->get<std::string>(MongoData::mailVerification);

			unverifiedMailUsers.emplace_back(user);
		}

		if (response.cursorID() == 0)
		{
			break;
		}

		response = cursor.next(connection);
	}

	return unverifiedMailUsers;
}

std::vector<User> Mongo::adminGetAllAdmins()
{
	Poco::MongoDB::Cursor cursor(MongoConfig::DbName, MongoConfig::CollectionName);
	cursor.query().selector().add(MongoData::status, MongoData::params::STATUS_ADMIN);
	Poco::MongoDB::ResponseMessage &response = cursor.next(connection);

	std::vector<User> unverifiedUsers;
	User user;

	for (;;)
	{
		for (Poco::MongoDB::Document::Vector::const_iterator it = response.documents().begin(); it != response.documents().end(); ++it)
		{
			user.username = (*it)->get<std::string>(MongoData::username);
			user.mail = (*it)->get<std::string>(MongoData::mail);
			user.status = (*it)->get<std::string>(MongoData::status);
			user.verification = (*it)->get<std::string>(MongoData::verification);
			user.mailVerification = (*it)->get<std::string>(MongoData::mailVerification);

			unverifiedUsers.emplace_back(user);
		}

		if (response.cursorID() == 0)
		{
			break;
		}

		response = cursor.next(connection);
	}

	return unverifiedUsers;
}

// INSERT INTO users(username, hashPassword, mail) VALUES(..., ...)
void Mongo::adminAddUser(const User& user)
{
	Poco::MongoDB::Database db(MongoConfig::DbName);
	Poco::SharedPtr<Poco::MongoDB::InsertRequest> insertPlayerRequest = db.createInsertRequest(MongoConfig::CollectionName);
	insertPlayerRequest->addNewDocument()
		.add(MongoData::username, user.username)
		.add(MongoData::mail, user.mail)
		.add(MongoData::hashPassword, user.hashPassword)
		.add(MongoData::status, MongoData::params::STATUS_USER)
		.add(MongoData::verification, MongoData::params::VERIFICATION_TRUE)
		.add(MongoData::mailVerification, MongoData::params::MAIL_VERIFICATION_FALSE);

	connection.sendRequest(*insertPlayerRequest);
	std::string lastError = db.getLastError(connection);
	if (!lastError.empty())
	{
		throw Poco::ApplicationException(db.getLastError(connection));
	}
}

// INSERT INTO users(username, hashPassword, mail) VALUES(..., ...)
void Mongo::adminAddAdmin(const User& user)
{
	Poco::MongoDB::Database db(MongoConfig::DbName);
	Poco::SharedPtr<Poco::MongoDB::InsertRequest> insertPlayerRequest = db.createInsertRequest(MongoConfig::CollectionName);
	insertPlayerRequest->addNewDocument()
		.add(MongoData::username, user.username)
		.add(MongoData::mail, user.mail)
		.add(MongoData::hashPassword, user.hashPassword)
		.add(MongoData::status, MongoData::params::STATUS_ADMIN)
		.add(MongoData::verification, MongoData::params::VERIFICATION_TRUE)
		.add(MongoData::mailVerification, MongoData::params::MAIL_VERIFICATION_FALSE);

	connection.sendRequest(*insertPlayerRequest);
	std::string lastError = db.getLastError(connection);
	if (!lastError.empty())
	{
		throw Poco::ApplicationException(db.getLastError(connection));
	}
}

// DELETE FROM UsersDb WHERE username = "..."
void Mongo::adminDeleteUser(const std::string& username)
{
	Poco::MongoDB::Database db(MongoConfig::DbName);
	Poco::SharedPtr<Poco::MongoDB::DeleteRequest> request = db.createDeleteRequest(MongoConfig::CollectionName);
	request->selector().add(MongoData::username, username);
	connection.sendRequest(*request);
	Poco::MongoDB::Document::Ptr lastError = db.getLastErrorDoc(connection);
	if (lastError->isType<std::string>("err"))
	{
		throw Poco::ApplicationException(lastError->toString(2));
	}
}