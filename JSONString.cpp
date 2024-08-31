#include "JSONString.h"

JSONString::JSONString(const std::string &value) : value(value) {}

JSONValueType JSONString::getType() const
{
    return JSONValueType::STRING;
}

std::string JSONString::toString() const
{
    return "\"" + value + "\"";
}