#pragma once

#include "Poco/Net/MailMessage.h"
#include "Poco/Net/MailRecipient.h"
#include "Poco/Net/SMTPClientSession.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/SecureSMTPClientSession.h"
#include "Poco/Net/InvalidCertificateHandler.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/SecureStreamSocket.h"

#include "configsgmail.hpp"

class Mail
{
private:
    std::string recipient;
    std::string title;
    std::string msg;
public:
    Mail(const std::string& recipient, const std::string& title, const std::string& msg);
    Mail();
private:
    void sendMail() const;
    void connection() const;
};