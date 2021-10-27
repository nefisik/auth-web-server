#include "database/algorithms/include/algorithms.hpp"
#include "database/redis/include/configsJWT.hpp"

#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Timespan.h>
#include <Poco/DateTimeParser.h>
#include <Poco/DateTimeFormat.h>

std::string Auth::random_string(std::string::size_type length)
{
    static auto &chrs = "0123456789"
                        "abcdefghijklmnopqrstuvwxyz"
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::mt19937 rg{std::random_device{}()};
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

    std::string s;

    s.reserve(length);

    while (length--)
        s += chrs[pick(rg)];

    return s;
}

std::string Auth::sha256(const User &user)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, user.password.c_str(), user.password.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}

std::string Auth::create_refresh_token(const User &user)
{
    try
    {
        auto key = JWTparams::refreshTokenKey;
        auto jti = Auth::random_string(30);

        jwt::jwt_object obj{
            jwt::params::algorithm("HS256"),
            jwt::params::payload({{JWTparams::username, user.username},
                                  {JWTparams::jti, jti}}),
            jwt::params::secret(key)};

        auto enc_str = obj.signature();

        return enc_str;
    }
    catch (const jwt::TokenExpiredError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (const jwt::SignatureFormatError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (const jwt::DecodeError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (const jwt::VerificationError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (...)
    {
        throw Poco::ApplicationException();
    }
}

std::string Auth::create_access_token(const User &user)
{
    try
    {
        auto key = JWTparams::accessTokenKey;
        auto jti = Auth::random_string(30);

        auto iat_time = Poco::DateTime();
        auto exp_time = iat_time + Poco::Timespan(0, 0, JWTparams::accessTokenLifetimeMinutes, 0, 0);

        auto formatter = Poco::DateTimeFormatter();
        auto iat = formatter.format(iat_time.timestamp(), Poco::DateTimeFormat::SORTABLE_FORMAT);
        auto exp = formatter.format(exp_time, Poco::DateTimeFormat::SORTABLE_FORMAT);

        jwt::jwt_object obj{
            jwt::params::algorithm("HS256"),
            jwt::params::payload({{JWTparams::username, user.username},
                                  {JWTparams::jti, jti},
                                  {JWTparams::iat, iat},
                                  {JWTparams::exp, exp}}),
            jwt::params::secret(key)};

        auto enc_str = obj.signature();

        return enc_str;
    }
    catch (const jwt::TokenExpiredError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (const jwt::SignatureFormatError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (const jwt::DecodeError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (const jwt::VerificationError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (...)
    {
        throw Poco::ApplicationException();
    }
}

std::string Auth::create_access_token(const std::string &refreshToken)
{
    try
    {
        auto dec_refreshToken = jwt::decode(refreshToken, jwt::params::algorithms({"HS256"}), jwt::params::secret(JWTparams::refreshTokenKey));

        auto username = dec_refreshToken.payload().get_claim_value<std::string>(JWTparams::username);

        auto key = JWTparams::accessTokenKey;
        auto jti = Auth::random_string(30);

        auto iat_time = Poco::DateTime();
        auto exp_time = iat_time + Poco::Timespan(0, 0, JWTparams::accessTokenLifetimeMinutes, 0, 0);

        auto formatter = Poco::DateTimeFormatter();
        auto iat = formatter.format(iat_time.timestamp(), Poco::DateTimeFormat::SORTABLE_FORMAT);
        auto exp = formatter.format(exp_time, Poco::DateTimeFormat::SORTABLE_FORMAT);

        jwt::jwt_object accessToken{
            jwt::params::algorithm("HS256"),
            jwt::params::payload({{JWTparams::username, username},
                                  {JWTparams::jti, jti},
                                  {JWTparams::iat, iat},
                                  {JWTparams::exp, exp}}),
            jwt::params::secret(key)};

        auto enc_accessToken = accessToken.signature();

        return enc_accessToken;
    }
    catch (const jwt::TokenExpiredError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (const jwt::SignatureFormatError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (const jwt::DecodeError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (const jwt::VerificationError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (...)
    {
        throw Poco::ApplicationException();
    }
}

bool Auth::check_refresh_token(const std::string &refresh_token)
{
    try
    {
        auto dec_obj = jwt::decode(refresh_token,
                                   jwt::params::algorithms({"HS256"}),
                                   jwt::params::secret(JWTparams::refreshTokenKey),
                                   jwt::params::verify(true));

        return true;
    }
    catch (const jwt::TokenExpiredError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (const jwt::SignatureFormatError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (const jwt::DecodeError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (const jwt::VerificationError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (...)
    {
        throw Poco::ApplicationException();
    }
}

bool Auth::check_access_token(const std::string &access_token)
{
    try
    {
        auto dec_obj = jwt::decode(access_token,
                                   jwt::params::algorithms({"HS256"}),
                                   jwt::params::secret(JWTparams::accessTokenKey),
                                   jwt::params::verify(true));

        auto iat_time = Poco::DateTime();
        auto exp = dec_obj.payload().get_claim_value<std::string>(JWTparams::exp);

        auto dif = 0;
        auto exp_time = Poco::DateTimeParser::parse(Poco::DateTimeFormat::SORTABLE_FORMAT, exp, dif);
        if (iat_time < exp_time)
            return true;

        return false;
    }
    catch (const jwt::TokenExpiredError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (const jwt::SignatureFormatError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (const jwt::DecodeError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (const jwt::VerificationError &e)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
    catch (...)
    {
        throw Poco::ApplicationException();
    }
}
