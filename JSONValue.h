#ifndef JSON_VALUE_H
#define JSON_VALUE_H

#include <string>
#include <vector>

enum class JSONValueType
{
    STRING,
    NUMBER,
    BOOL,
    OBJECT,
    ARRAY,
    NILL
};

class JSONValue
{
public:
    virtual ~JSONValue() = default;
    virtual JSONValueType getType() const = 0;
    virtual std::string toString() const = 0; 
};

#endif