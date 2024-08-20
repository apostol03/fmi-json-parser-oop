#ifndef JSON_NULL_H
#define JSON_NULL_H

#include "JSONValue.h"

class JSONNull : public JSONValue
{
public:
    JSONValueType getType() const override
    {
        return JSONValueType::NILL;
    }

    std::string toString() const override
    {
        return "null";
    }
};

#endif