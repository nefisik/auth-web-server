#include "database/mail/include/mail.hpp"

Mail::Mail(const std::string& recipient, const std::string& title, const std::string& msg)
    :recipient(recipient), title(title), msg(msg)
{
    sendMail();
}

Mail::Mail()
{
    connection();
}

void Mail::connection() const
{
    Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> pCert = new Poco::Net::AcceptCertificateHandler(false);
    Poco::Net::Context::Ptr pContext = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
    Poco::Net::SSLManager::instance().initializeClient(0, pCert, pContext);

    // Для отправки gmail на 465 порт раскомментировать:
    //
    // Poco::Net::SecureStreamSocket pSSLSocket(pContext);
    // pSSLSocket.connect(Poco::Net::SocketAddress(GMailConfig::host, GMailConfig::port));
    // Poco::Net::SecureSMTPClientSession secure(pSSLSocket);
    //
    // Строку ниже для 465 порта закомментировать
    Poco::Net::SecureSMTPClientSession secure(GMailConfig::host, GMailConfig::port);
}

void Mail::sendMail() const
{
    Poco::Net::MailMessage message;

    Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> pCert = new Poco::Net::AcceptCertificateHandler(false);
    Poco::Net::Context::Ptr pContext = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
    Poco::Net::SSLManager::instance().initializeClient(0, pCert, pContext);

    Poco::Net::SecureSMTPClientSession secure(GMailConfig::host, GMailConfig::port);

    secure.login();
    bool tlsStarted = secure.startTLS(pContext);
    secure.login(Poco::Net::SMTPClientSession::AUTH_LOGIN, GMailConfig::senderId, GMailConfig::senderPwd);

    try
    {
        message.setSender(GMailConfig::senderMail);
        message.addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT, recipient));
        message.setSubject(title);
        message.setContentType("text/plain; charset=UTF-8");
        message.setContent(msg, Poco::Net::MailMessage::ENCODING_8BIT);

        secure.sendMessage(message);
        secure.close();
    }
    catch (...)
    {
        throw Poco::Exception("Указана недействительная почта", 400);
    }
}
