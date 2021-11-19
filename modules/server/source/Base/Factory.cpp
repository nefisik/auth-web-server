#include "server/include/Base/Factory.hpp"

bool Factory::methodURI(const std::string& valid_uri, const std::string& now_uri)
{
    if (valid_uri.size() != now_uri.size())
	{
		return false;	
	}

    bool key = true;
    for (size_t i = 0; i < now_uri.size(); i++)
	{
        if (valid_uri[i] != now_uri[i])
		{
			key = false;
		}
    }

    return key;
}

Poco::Net::HTTPRequestHandler* Factory::createRequestHandler(const Request& request)
{
	const Poco::URI uri(request.getURI());

	try
	{

/********************************************AUTH_METHODS************************************************/

        if (methodURI(AuthSignUpURL, request.getURI().substr(0, AuthSignUpURL.size())))
            return new AuthHandler(AuthCommands::SIGN_UP);

        else if (methodURI(SignUpVerifyURL, request.getURI().substr(0, SignUpVerifyURL.size())))
            return new AuthHandler(AuthCommands::SIGN_UP_VERIFY_URL);

        else if (methodURI(AuthSignInURL, request.getURI().substr(0, AuthSignInURL.size())))
            return new AuthHandler(AuthCommands::SIGN_IN);

        else if (methodURI(AuthRefreshTokenURL, request.getURI().substr(0, AuthRefreshTokenURL.size())))
            return new AuthHandler(AuthCommands::REFRESH);

        else if (methodURI(AuthSignOutURL, request.getURI().substr(0, AuthSignOutURL.size())))
            return new AuthHandler(AuthCommands::SIGN_OUT);

        else if (methodURI(MailPasswordRecoveryURL, request.getURI().substr(0, MailPasswordRecoveryURL.size())))
            return new AuthHandler(AuthCommands::MAIL_PASSWORD_RECOVERY);

        else if (methodURI(CheckRecoveryTokenURL, request.getURI().substr(0, CheckRecoveryTokenURL.size())))
            return new AuthHandler(AuthCommands::CHECK_RECOVERY_TOKEN);

        else if (methodURI(PasswordRecoveryURL, request.getURI().substr(0, PasswordRecoveryURL.size())))
            return new AuthHandler(AuthCommands::PASSWORD_RECOVERY);

/*********************************************ADMIN_PANEL************************************************/

        else if (methodURI(UpdateDataURL, request.getURI().substr(0, UpdateDataURL.size())))
            return new AdminHandler(AdminCommands::UPDATE_DATA);

        else if (methodURI(GetAllUsersURL, request.getURI().substr(0, GetAllUsersURL.size())))
            return new AdminHandler(AdminCommands::GET_ALL_USERS);

        else if (methodURI(GetAllUnverifiedUsersURL, request.getURI().substr(0, GetAllUnverifiedUsersURL.size())))
            return new AdminHandler(AdminCommands::GET_ALL_UNVERIFIED_USERS);

        else if (methodURI(GetAllUnverifiedMailUsersURL, request.getURI().substr(0, GetAllUnverifiedMailUsersURL.size())))
            return new AdminHandler(AdminCommands::GET_ALL_UNVERIFIED_MAIL_USERS);

        else if (methodURI(GetAllAdminsURL, request.getURI().substr(0, GetAllAdminsURL.size())))
            return new AdminHandler(AdminCommands::GET_ALL_ADMINS);

        else if (methodURI(SearchUserURL, request.getURI().substr(0, SearchUserURL.size())))
            return new AdminHandler(AdminCommands::SEARCH_USER);

        else if (methodURI(AddUserURL, request.getURI().substr(0, AddUserURL.size())))
            return new AdminHandler(AdminCommands::ADD_USER);

        else if (methodURI(AddAdminURL, request.getURI().substr(0, AddAdminURL.size())))
            return new AdminHandler(AdminCommands::ADD_ADMIN);

        else if (methodURI(DeleteUserURL, request.getURI().substr(0, DeleteUserURL.size())))
            return new AdminHandler(AdminCommands::DELETE_USER);

        else
            return new ErrorHandler(Status::HTTP_NOT_FOUND);

    }
	catch (...)
	{
		return new ErrorHandler(Status::HTTP_INTERNAL_SERVER_ERROR);
	}

	return nullptr;
}