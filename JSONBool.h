#ifndef JSON_BOOL_H
#define JSON_BOOL_H

#include "JSONValue.h"

class JSONBool : public JSONValue
{
public:
    JSONBool(bool value) : value(value) {}

    JSONValueType getType() const override 
    {
        return JSONValueType::BOOL;
    }

    std::string toString() const override
    {
        return value ? "true" : "false";
    }

private:
    bool value;
};

#endif