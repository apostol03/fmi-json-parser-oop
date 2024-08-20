#ifndef JSON_ARRAY_H
#define JSON_ARRAY_H

#include "JSONValue.h"

class JSONArray : public JSONValue
{
public:
    JSONValueType getType() const override
    {
        return JSONValueType::ARRAY;
    }

    std::string toString() const override
    {
        std::string result = "[";
        for (size_t i = 0; i < values.size(); i++)
        {
            if (i > 0)
            {
                result += ", ";
            }

            result += values[i]->toString();
        }
        result += "]";
        return result;
    }

    void addValue(JSONValue *value)
    {
        values.push_back(value);
    }

    std::vector<JSONValue *> getValues() const
    {
        return values;
    }

private:
    std::vector<JSONValue *> values;
};

#endif