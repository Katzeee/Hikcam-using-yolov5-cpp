#include "jsondeal.h"

JsonDeal::JsonDeal()
{
    // reader = std::make_unique<Json::CharReader>(builder.newCharReader());
}

bool JsonDeal::cvtSting2Json(const std::string &rawJson, Json::Value &result, std::string &errorMessage)
{
    std::unique_ptr<Json::CharReader> reader(charReaderBuilder.newCharReader());
    auto rawJsonLength = static_cast<int>(rawJson.length());
    JSONCPP_STRING err;
    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &result, &err))
    {
        errorMessage = err;
        return false;
    }
    return true;
}

bool JsonDeal::cvtJson2String(const Json::Value &root, std::string &result)
{
    try
    {
        result = Json::writeString(streamWriterBuilder, root);
    }
    catch (...)
    {
        return false;
    }
    return true;
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
