#ifndef JSON_NUMBER_H
#define JSON_NUMBER_H

#include <cmath>

#include "JSONValue.h"

class JSONNumber : public JSONValue
{
public:
    JSONNumber(double value) : value(value) {}

    JSONValueType getType() const override
    {
        return JSONValueType::NUMBER;
    }

    std::string toString() const override
    {
        if (value == std::floor(value))
        {
            return std::to_string(static_cast<int>(value));
        }

        return std::to_string(value);
    }

private:
    double value;
};

#endif