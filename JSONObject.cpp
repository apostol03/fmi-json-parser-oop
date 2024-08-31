#include "JSONObject.h"

JSONValueType JSONObject::getType() const
{
    return JSONValueType::OBJECT;
}

std::string JSONObject::toString() const
{
    std::string result = "{\n";
    for (size_t i = 0; i < values.size(); i++)
    {
        if (i > 0)
        {
            result += ", \n";
        }
        result += "  \"" + values[i].key + "\": " + values[i].value->toString();
    }
    result += "\n}";
    return result;
}

void JSONObject::addValue(const std::string &key, JSONValue *value)
{
    values.emplace_back(key, value);
}

std::vector<KeyValue> JSONObject::getValues() const
{
    return values;
}

void JSONObject::setValue(const std::string &key, JSONValue *newValue)
{
    for (auto &keyValue : values)
    {
        if (keyValue.key == key)
        {
            delete keyValue.value;
            keyValue.value = newValue;
            return;
        }
    }

    values.emplace_back(key, newValue);
}

JSONValue *JSONObject::getValue(const std::string &key)
{
    for (auto &keyValue : values)
    {
        if (keyValue.key == key)
        {
            return keyValue.value;
        }
    }

    return nullptr;
}

void JSONObject::removeValue(const std::string &key)
{
    for (auto it = values.begin(); it != values.end(); ++it)
    {
        if (it->key == key)
        {
            delete it->value;
            values.erase(it);
            return;
        }
    }
}