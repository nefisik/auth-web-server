#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <openssl/sha.h>
#include "database/mongodb/include/user.h"

void sha256(User & user);
