#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

#include "JSONValue.h"

struct KeyValue
{
    std::string key;
    JSONValue *value;

    KeyValue(const std::string &key, JSONValue *value) : key(key), value(value) {}
};

class JSONObject : public JSONValue
{
public:
    JSONValueType getType() const override
    {
        return JSONValueType::OBJECT;
    }

    std::string toString() const override
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

    void addValue(const std::string &key, JSONValue *value)
    {
        values.emplace_back(key, value);
    }

    std::vector<KeyValue> getValues() const 
    {
        return values;
    }

    void setValue(const std::string &key, JSONValue *newValue)
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

    JSONValue *getValue(const std::string &key)
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

private:
    std::vector<KeyValue> values;
};

#endif