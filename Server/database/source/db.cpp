#include "include/db.h"

void registration(Session& session, User& user) {
	try{
		Statement insert(session);
		insert << "INSERT INTO Users(username, hash_password) VALUES('" +
			user.username + "', '" + user.password + "')", now;

		insert << "INSERT INTO Tokens(token) VALUES('" + user.token + "')", now;

		insert << "INSERT INTO Tokens_id(user_id, token_id) \
			VALUES((SELECT user_id FROM Users WHERE username = '" +
					user.username + "'), \
			(SELECT token_id FROM Tokens WHERE Tokens WHERE token = '" +
					user.token + "'))", now;
	}
	catch(Poco::Data::PostgreSQL::StatementException &exc)
	{
		std::cerr << exc.displayText() <<std::endl;
		return;
	}
}

void authentication(Session& session, User& user) {
	try
	{
		Statement select(session);
		select << "SELECT hash_password FROM Users WHERE username = '"
				+ user.username + "'", into(pswd), now;
		
		select << "UPDATE Tokens SET token = '" + user.token + "' FROM Users, Tokens_id \
			WHERE Users.username = '" + user.username + "' AND \
			Users.user_id = Tokens_id.user_id AND \
			Tokend_id.token_id = Tokens.token_id;", now;

		select << "SELECT token FROM Users, Tokens_id, Tokens \
			WHERE Users.username = '" + user.username + "' AND \
			Users.user.id = Tokens_id.user_id AND \
			Tokend_id.token_id = Tokend.token_id;", into(user.token), now;
	}
	catch(Poco::Data::PostgreSQL::StatementException& exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return;
	}
}

int main()
{
	Poco::Data::PostgreSQL::Connector::registerConnector();

	std::string host = "localhost";
	std::string port = "5432";
	std::string db = "auth";
	std::string user = "postgres";
	std::string password = "user";

	std::string connectionString = "host=" + host + " user=" + user + " password" + password + " dbname=" + db;

	Session session("PostgreSQL", connectionString);

	User usr;
	usr.username = "Maria";
	usr.password = "23403jferf";

	registration(session, usr);
	authentiaction(session, usr);
}