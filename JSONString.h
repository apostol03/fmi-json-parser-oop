#ifndef JSON_STRING_H
#define JSON_STRING_H

#include "JSONValue.h"

class JSONString : public JSONValue
{
public:
    JSONString(const std::string &value) : value(value) {}
    
    JSONValueType getType() const override 
    {
        return JSONValueType::STRING;
    }

    std::string toString() const override
    {
        return "\"" + value + "\"";
    }

private:
    std::string value;
};

#endif