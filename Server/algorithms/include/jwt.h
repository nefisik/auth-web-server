#pragma once

#include <iostream>
#include "jwt/jwt.hpp"
#include "database/mongodb/include/user.h"
#include "database/mongodb/include/mongodata.h"

void create_token(User& user);