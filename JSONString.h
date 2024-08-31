#ifndef JSON_STRING_H
#define JSON_STRING_H

#include "JSONValue.h"

class JSONString : public JSONValue
{
public:
    JSONString(const std::string &value);
    
    JSONValueType getType() const override;

    std::string toString() const override;
private:
    std::string value;
};

#endif