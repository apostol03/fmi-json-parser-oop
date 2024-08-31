#include "JSONArray.h"

JSONValueType JSONArray::getType() const
{
    return JSONValueType::ARRAY;
}

std::string JSONArray::toString() const
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

void JSONArray::addValue(JSONValue *value)
{
    values.push_back(value);
}

std::vector<JSONValue *> JSONArray::getValues() const
{
    return values;
}