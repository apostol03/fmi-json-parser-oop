#ifndef JSON_BOOL_H
#define JSON_BOOL_H

#include "JSONValue.h"

class JSONBool : public JSONValue
{
public:
    JSONBool(bool value);
    JSONValueType getType() const override;
    std::string toString() const override;
    
private:
    bool value;
};

#endif