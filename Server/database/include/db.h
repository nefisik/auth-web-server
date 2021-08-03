#pragma once

#include "dbstructure.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/PostgreSQL/Connector.h"
#include "Poco/Data/PostgreSQL/PostgreSQLException.h"
#include <vector>
#include <iostream>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

void registration(Session& session, User& user);
void authentiaction(Session& session, User& user);

//g++ db.cpp -o db -lPocoDataPostgreSQL -lPocoData -lPocoFoundation