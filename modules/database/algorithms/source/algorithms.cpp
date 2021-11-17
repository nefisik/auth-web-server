#include "database/algorithms/include/algorithms.hpp"
#include "database/redis/include/redisdata.hpp"

#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Timespan.h>
#include <Poco/DateTimeParser.h>
#include <Poco/DateTimeFormat.h>

std::string Algo::randomString(std::string::size_type length)
{
    static auto &chrs = "0123456789"
                        "abcdefghijklmnopqrstuvwxyz"
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::mt19937 rg{std::random_device{}()};
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

    std::string s;

    s.reserve(length);

    while (length--)
    {
        s += chrs[pick(rg)];
    }

    return s;
}

std::string Algo::sha256(const std::string& password)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password.c_str(), password.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}

int Algo::minDistance(std::string word1, std::string word2)
{
	// Transform words to lowercase
	std::transform(word1.begin(), word1.end(), word1.begin(), tolower);
	std::transform(word2.begin(), word2.end(), word2.begin(), tolower);

	// Search for a subline in a string
	size_t pos = word1.find(word2);
	if (pos != std::string::npos)
	{
		if (pos == 0)
			return -1;
		else
			return 0;
	}

	// Determine which string is shorter.
	const std::string *short_ptr;
	const std::string *long_ptr;
	if (word1.length() < word2.length())
	{
		short_ptr = &word1;
		long_ptr = &word2;
	}
	else
	{
		short_ptr = &word2;
		long_ptr = &word1;
	}

	// O(min(m, n))
	int cache[short_ptr->length() + 1];
	for (int i = 0; i < short_ptr->length() + 1; i++)
	{
		cache[i] = i;
	}

	// tmp1: storing the most recently overridden element of cache
	// tmp2: storing the currently calculated value
	int tmp1, tmp2;

	for (int i = 1; i < long_ptr->length() + 1; i++)
	{
		tmp1 = cache[0];
		cache[0] = i;
		for (int j = 1; j < short_ptr->length() + 1; j++)
		{
			if (long_ptr->at(i - 1) == short_ptr->at(j - 1))
			{
				// Two strings have the same final character for this sub-problem
				tmp2 = tmp1;
				tmp1 = cache[j];
				cache[j] = tmp2;
			}
			else
			{
				tmp2 = std::min(std::min(tmp1, cache[j - 1]), cache[j]) + 1;
				tmp1 = cache[j];
				cache[j] = tmp2;
			}
		}
	}
	return cache[short_ptr->length()];
}

std::vector<User> Algo::searchResult(const std::vector<User> users, const std::string search)
{
	std::vector<User> result;
    std::vector<std::pair<int, User>> resultVec;

	for (const auto& user : users)
	{
		auto dist = minDistance(user.username, search); 
		if (dist < user.username.size() * 0.6)
		{
			resultVec.emplace_back(std::make_pair(dist, user));
		}
	}

    std::sort(resultVec.begin(), resultVec.end(), [](auto &left, auto &right) {
        return left.first < right.first;
    });

    for (const auto& user : resultVec)
    {
        result.emplace_back(user.second);
    }

	return result;
}

std::string Auth::createRefreshToken(const User& user)
{
    const auto key = JWTparams::refreshTokenKey;
    const auto jti = Algo::randomString(30);

    const jwt::jwt_object obj
    {
        jwt::params::algorithm("HS256"),
        jwt::params::payload({{JWTparams::username, user.username},
                              {JWTparams::status, user.status},
                              {JWTparams::jti, jti}}),
        jwt::params::secret(key)
    };

    auto enc_str = obj.signature();

    return enc_str;
}

std::string Auth::createAccessToken(const User& user)
{
    const auto key = JWTparams::accessTokenKey;
    const auto jti = Algo::randomString(30);

    const auto iat_time = Poco::DateTime();
    const auto exp_time = iat_time + Poco::Timespan(0, 0, JWTparams::accessTokenLifetimeMinutes, 0, 0);

    const auto formatter = Poco::DateTimeFormatter();
    const auto iat = formatter.format(iat_time.timestamp(), Poco::DateTimeFormat::SORTABLE_FORMAT);
    const auto exp = formatter.format(exp_time, Poco::DateTimeFormat::SORTABLE_FORMAT);

    const jwt::jwt_object obj
    {
        jwt::params::algorithm("HS256"),
        jwt::params::payload({{JWTparams::username, user.username},
                              {JWTparams::status, user.status},
                              {JWTparams::jti, jti},
                              {JWTparams::iat, iat},
                              {JWTparams::exp, exp}}),
        jwt::params::secret(key)
    };

    const auto enc_str = obj.signature();

    return enc_str;
}

std::string Auth::createAccessToken(const std::string& refreshToken)
{
    try
    {
        const auto dec_refreshToken = jwt::decode(refreshToken, jwt::params::algorithms({"HS256"}), jwt::params::secret(JWTparams::refreshTokenKey));

        const auto username = dec_refreshToken.payload().get_claim_value<std::string>(JWTparams::username);
        const auto status = dec_refreshToken.payload().get_claim_value<std::string>(JWTparams::status);

        const auto key = JWTparams::accessTokenKey;
        const auto jti = Algo::randomString(30);

        const auto iat_time = Poco::DateTime();
        const auto exp_time = iat_time + Poco::Timespan(0, 0, JWTparams::accessTokenLifetimeMinutes, 0, 0);

        const auto formatter = Poco::DateTimeFormatter();
        const auto iat = formatter.format(iat_time.timestamp(), Poco::DateTimeFormat::SORTABLE_FORMAT);
        const auto exp = formatter.format(exp_time, Poco::DateTimeFormat::SORTABLE_FORMAT);

        const jwt::jwt_object accessToken
        {
            jwt::params::algorithm("HS256"),
            jwt::params::payload({{JWTparams::username, username},
                                  {JWTparams::status, status},
                                  {JWTparams::jti, jti},
                                  {JWTparams::iat, iat},
                                  {JWTparams::exp, exp}}),
            jwt::params::secret(key)
        };

        const auto enc_accessToken = accessToken.signature();

        return enc_accessToken;
    }
    catch(...)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
}

std::string Auth::createRecoveryToken(const User& user)
{
    const auto key = JWTparams::recoveryTokenKey;
    const auto jti = Algo::randomString(30);

    const auto iat_time = Poco::DateTime();
    const auto exp_time = iat_time + Poco::Timespan(0, 0, JWTparams::recoveryTokenLifetimeMinutes, 0, 0);

    const auto formatter = Poco::DateTimeFormatter();
    const auto iat = formatter.format(iat_time.timestamp(), Poco::DateTimeFormat::SORTABLE_FORMAT);
    const auto exp = formatter.format(exp_time, Poco::DateTimeFormat::SORTABLE_FORMAT);

    const jwt::jwt_object obj
    {
        jwt::params::algorithm("HS256"),
        jwt::params::payload({{JWTparams::username, user.username},
                              {JWTparams::status, user.status},
                              {JWTparams::jti, jti},
                              {JWTparams::iat, iat},
                              {JWTparams::exp, exp}}),
        jwt::params::secret(key)
    };

    const auto enc_str = obj.signature();

    return enc_str;
}

std::string Auth::createSignUpToken(const User& user)
{
    const auto key = JWTparams::signUpTokenKey;
    const auto jti = Algo::randomString(30);

    const auto iat_time = Poco::DateTime();

    const auto formatter = Poco::DateTimeFormatter();
    const auto iat = formatter.format(iat_time.timestamp(), Poco::DateTimeFormat::SORTABLE_FORMAT);

    const jwt::jwt_object obj
    {
        jwt::params::algorithm("HS256"),
        jwt::params::payload({{JWTparams::username, user.username},
                              {JWTparams::status, user.status},
                              {JWTparams::jti, jti},
                              {JWTparams::iat, iat}}),
        jwt::params::secret(key)
    };

    const auto enc_str = obj.signature();

    return enc_str;
}

bool Auth::checkRefreshToken(const std::string& refreshToken)
{
    try
    {
        const auto dec_obj = jwt::decode(refreshToken,
                                    jwt::params::algorithms({"HS256"}),
                                    jwt::params::secret(JWTparams::refreshTokenKey),
                                    jwt::params::verify(true));

        return true;
    }
    catch(...)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
}

bool Auth::checkAccessToken(const std::string& accessToken, const std::string& status)
{
    try
    {
        const auto dec_obj = jwt::decode(accessToken,
                                    jwt::params::algorithms({"HS256"}),
                                    jwt::params::secret(JWTparams::accessTokenKey),
                                    jwt::params::verify(true));

        const auto iat_time = Poco::DateTime();
        const auto exp = dec_obj.payload().get_claim_value<std::string>(JWTparams::exp);

        auto dif = 0;
        const auto exp_time = Poco::DateTimeParser::parse(Poco::DateTimeFormat::SORTABLE_FORMAT, exp, dif);

        if (iat_time > exp_time)
        {
            return false;
        }

        if (status == MongoData::params::STATUS_ADMIN)
        {
            const auto token_status = dec_obj.payload().get_claim_value<std::string>(JWTparams::status);
            if (token_status != MongoData::params::STATUS_ADMIN)
            {
                return false;
            }
        }

        return true;
    }
    catch(...)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
}

std::string Auth::checkRecoveryToken(const std::string& recoveryToken)
{
    try
    {
        const auto dec_obj = jwt::decode(recoveryToken,
                                    jwt::params::algorithms({"HS256"}),
                                    jwt::params::secret(JWTparams::recoveryTokenKey),
                                    jwt::params::verify(true));

        const auto iat_time = Poco::DateTime();
        const auto exp = dec_obj.payload().get_claim_value<std::string>(JWTparams::exp);

        const auto username = dec_obj.payload().get_claim_value<std::string>(JWTparams::username);

        auto dif = 0;
        const auto exp_time = Poco::DateTimeParser::parse(Poco::DateTimeFormat::SORTABLE_FORMAT, exp, dif);

        if (iat_time > exp_time)
        {
            throw Poco::Net::NotAuthenticatedException("Unauthorized");
        }

        return username;
    }
    catch(...)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
}

std::string Auth::checkSignUpToken(const std::string& signUpToken)
{
    try
    {
        const auto dec_obj = jwt::decode(signUpToken,
                                    jwt::params::algorithms({"HS256"}),
                                    jwt::params::secret(JWTparams::signUpTokenKey),
                                    jwt::params::verify(true));

        const auto username = dec_obj.payload().get_claim_value<std::string>(JWTparams::username);

        return username;
    }
    catch(...)
    {
        throw Poco::Net::NotAuthenticatedException("Unauthorized");
    }
}