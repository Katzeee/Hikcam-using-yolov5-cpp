#include <json/json.h>
#include <string>
#include <iostream>
#include <memory>

class JsonDeal
{
public:
    JsonDeal();
    Json::Value cvtSting2Json(const std::string rawJson, bool &success, std::string &errorMessage);
    std::string cvtJson2String(const Json::Value &root, bool &success);
    bool cvtJson2Stream(const Json::Value &root, Json::OStream *sout);
private:
    Json::Value root;
    Json::Reader reader;
    Json::StreamWriterBuilder streamWriterBuilder;
    Json::CharReaderBuilder charReaderBuilder;
};