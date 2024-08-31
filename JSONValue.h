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

    /**
     * Returns the type of the JSON value.
     */
    virtual JSONValueType getType() const = 0;

    /**
     * Converts the JSON value into a string.
     */
    virtual std::string toString() const = 0; 
};

#endif