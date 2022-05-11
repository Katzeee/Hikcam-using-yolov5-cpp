#pragma once
#include <iostream>
#include <chrono>
#include <json/json.h>
#include <string>
#include <memory>

class JsonDeal
{
public:
    JsonDeal();
    bool cvtSting2Json(const std::string &rawJson, Json::Value &result, std::string &errorMessage);
    bool cvtJson2String(const Json::Value &root, std::string &result);
    bool cvtJson2Stream(const Json::Value &root, Json::OStream *sout);
private:
    Json::StreamWriterBuilder streamWriterBuilder;
    Json::CharReaderBuilder charReaderBuilder;
};

inline std::chrono::milliseconds utlsGetCurMiliStamp()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}