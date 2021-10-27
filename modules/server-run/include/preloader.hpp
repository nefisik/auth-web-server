#pragma once

#include <string>
#include <iostream>

#include <boost/filesystem.hpp>
#include <Poco/Exception.h>
#include <Poco/MD5Engine.h>
#include <Poco/DigestStream.h>

std::string getHashSum(const std::string &fileContent);

void WriteOrRewriteConfig();

void mayBeNotWriteConfig();

std::string getConfigValue(const std::string &val_name);

void checkMongoDbConnection();

void checkRedisDbConnection();

void starter();