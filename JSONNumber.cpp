#include "JSONNumber.h"

JSONNumber::JSONNumber(double value) : value(value) {}

JSONValueType JSONNumber::getType() const
{
    return JSONValueType::NUMBER;
}

std::string JSONNumber::toString() const
{
    if (value == std::floor(value))
    {
        return std::to_string(static_cast<int>(value));
    }

    return std::to_string(value);
}