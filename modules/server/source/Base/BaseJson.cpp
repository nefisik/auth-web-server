#include "server/include/Base/BaseJson.hpp"

BaseJson::BaseJson(int status)
{
    DateGenerator();
    new_json["CONTENT-TYPE"] = "application/json";
    new_json["RESPONSE"]["STATUS"] = "200";
    new_json["RESPONSE"]["COMMENT"] = "OK";
}

BaseJson::~BaseJson() = default;

void BaseJson::DateGenerator()
{
    Poco::LocalDateTime time;
    Poco::DateTimeFormatter formatter;
    new_json["Date Time"] = formatter.format(time.timestamp(), "%Y.%n.%d %H:%M:%S");
}