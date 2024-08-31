#ifndef JSON_NULL_H
#define JSON_NULL_H

#include "JSONValue.h"

class JSONNull : public JSONValue
{
public:
    JSONValueType getType() const override;
    std::string toString() const override;
};

#endif