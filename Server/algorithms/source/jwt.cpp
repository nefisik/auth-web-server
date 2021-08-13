#include "algorithms/include/jwt.h"

void create_token(User &user)
{
	using namespace jwt::params;

	auto key = user.username + user.status; //Secret to use for the algorithm
	//Create JWT object
	jwt::jwt_object obj{algorithm("HS256"),
						payload({{MongoData::username, user.username,}, {MongoData::status, user.status}}),
						secret(key)};

	//Get the encoded string/assertion
	auto enc_str = obj.signature();
	//   std::cout << enc_str << std::endl;

	//Decode
	auto dec_obj = jwt::decode(enc_str, algorithms({"HS256"}), secret(key));
	//   std::cout << dec_obj.header() << std::endl;
	//   std::cout << dec_obj.payload() << std::endl;

	user.token = enc_str;
}