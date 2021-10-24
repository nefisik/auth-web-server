#pragma once

#include <string>

std::string getHashSum(const std::string &fileContent);

void WriteOrRewriteConfig();

void mayBeNotWriteConfig();

std::string getConfigValue(const std::string &val_name);
