#pragma once

#include <string>
#include "database/mongodb/include/user.hpp"
#include "handlers/include/Base/BaseHandler.hpp"

class AuthMethods
{
public:

    void SIGN_UP(User user);

    std::pair<std::string, std::string> SIGN_IN(User user);

    void SIGN_OUT(std::string refreshToken);

    std::string REFRESH(std::string refreshToken);
};
