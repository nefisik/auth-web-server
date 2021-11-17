#pragma once

#include <string>

//__________CORS___________________
const static std::string CORS = "*";

//_______________AUTHORIZATION____________________________
const static std::string AuthSignUpURL = "/auth/signup/";
const static std::string SignUpVerifyURL = "/auth/signUpVerifyURL/";
const static std::string AuthSignInURL = "/auth/signin/";
const static std::string AuthRefreshTokenURL = "/auth/refresh/";
const static std::string AuthSignOutURL = "/auth/signout/";
const static std::string MailPasswordRecoveryURL = "/auth/mailPasswordRecovery/";
const static std::string CheckRecoveryTokenURL = "/auth/checkRecoveryToken/";
const static std::string PasswordRecoveryURL = "/auth/passwordRecovery/";

//________________ADMIN_PANEL_____________________________
const static std::string UpdateDataURL = "/admin/updateData/";
const static std::string GetAllUsersURL = "/admin/getAllUsers/";
const static std::string GetAllUnverifiedUsersURL = "/admin/getAllUnverifiedUsers/";
const static std::string GetAllUnverifiedMailUsersURL = "/admin/getAllUnverifiedMailUsers/";
const static std::string GetAllAdminsURL = "/admin/getAllAdmins/";
const static std::string SearchUserURL = "/admin/searchUser/";
const static std::string AddUserURL = "/admin/addUser/";
const static std::string AddAdminURL = "/admin/addAdmin/";
const static std::string DeleteUserURL = "/admin/deleteUser/";