#include "Searcher.h"

std::vector<JSONValue *> Searcher::searchByKey(const JSONObject *jsonObject, const std::string &key)
{
    std::vector<JSONValue *> results;
    searchObject(jsonObject, key, results);
    return results;
}

void Searcher::searchObject(const JSONObject *jsonObject, const std::string &key, std::vector<JSONValue *> &results)
{
    for (const auto &keyValue : jsonObject->getValues())
    {
        if (keyValue.key == key)
        {
            results.push_back(keyValue.value);
        }

        if (keyValue.value->getType() == JSONValueType::OBJECT)
        {
            searchObject(static_cast<const JSONObject *>(keyValue.value), key, results);
        }
        else if (keyValue.value->getType() == JSONValueType::ARRAY)
        {
            searchArray(static_cast<const JSONArray *>(keyValue.value), key, results);
        }
    }
}

void Searcher::searchArray(const JSONArray *jsonArray, const std::string &key, std::vector<JSONValue *> &results)
{
    for (const auto &value : jsonArray->getValues())
    {
        if (value->getType() == JSONValueType::OBJECT)
        {
            searchObject(static_cast<const JSONObject *>(value), key, results);
        }
        else if (value->getType() == JSONValueType::ARRAY)
        {
            searchArray(static_cast<const JSONArray *>(value), key, results);
        }
    }
}
