#include <string>

#include "server/include/Base/static/urls.hpp"
#include "server/include/preloader.hpp"

namespace GMailConfig
{
    const std::string host = Preloader::getConfigValue("GMailServerHost");
    const int port = stoi(Preloader::getConfigValue("GMailServerPort"));
    const std::string senderId = Preloader::getConfigValue("GMailSenderId");
    const std::string senderMail = Preloader::getConfigValue("GMailSenderMail");
    const std::string senderPwd = Preloader::getConfigValue("GMailSenderPwd");

} // GMailConfig

namespace HostLink
{
    const std::string host = Preloader::getConfigValue("Host");
    const std::string port = Preloader::getConfigValue("Port");

} // HostLink

namespace MessageTitle
{
    const std::string RECOVERY_PASSWORD = "Восстановление пароля";
    const std::string SIGN_UP_VERIFICATION = "Подтверждение регистрации";

} // MessageTitle

namespace MessageLink
{
    const std::string signUpVerifyLink = HostLink::host + std::string(":") + HostLink::port + std::string("/#") + SignUpVerifyURL;
    const std::string recoveryPwdLink = HostLink::host + std::string(":") + HostLink::port + std::string("/#") + CheckRecoveryTokenURL;

} // MessageLink