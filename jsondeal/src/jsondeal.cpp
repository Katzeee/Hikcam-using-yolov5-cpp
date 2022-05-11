#include "jsondeal.h"

JsonDeal::JsonDeal()
{
    // reader = std::make_unique<Json::CharReader>(builder.newCharReader());
}

Json::Value JsonDeal::cvtSting2Json(const std::string rawJson, bool &success, std::string &errorMessage)
{
    success = false;
    std::unique_ptr<Json::CharReader> reader(charReaderBuilder.newCharReader());
    auto rawJsonLength = static_cast<int>(rawJson.length());
    JSONCPP_STRING err;
    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &err))
    {
        errorMessage = err;
        success = false;
    }
    success = true;
}

std::string JsonDeal::cvtJson2String(const Json::Value &root, bool &success)
{
    std::string result;
    success = false;
    try
    {
        result = Json::writeString(streamWriterBuilder, root);
    }
    catch (...)
    {
        throw;
    }
    success = true;
    return result;
}

bool JsonDeal::cvtJson2Stream(const Json::Value &root, Json::OStream *sout)
{
    const std::unique_ptr<Json::StreamWriter> writer(streamWriterBuilder.newStreamWriter());
    try
    {
        writer->write(root, sout);
    }
    catch (...)
    {
        throw;
    }
    return true;
}
