#pragma once
#include <json/json.h>
#include <string>
#include <iostream>
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