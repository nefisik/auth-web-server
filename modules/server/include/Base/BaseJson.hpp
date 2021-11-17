#pragma once

#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>

#include "server/include/Base/libs/ArduinoJson.hpp"

class BaseJson
{
protected:
    ArduinoJson::StaticJsonDocument<1024 * 1024 * 1024> new_json;
    int _status;
public:
    BaseJson(int status = 200);

    ~BaseJson();

    void DateGenerator();

    void setStatus(int status);
};
