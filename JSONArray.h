#ifndef JSON_ARRAY_H
#define JSON_ARRAY_H

#include "JSONValue.h"

class JSONArray : public JSONValue
{
public:
    JSONValueType getType() const override;
    std::string toString() const override;

    /**
     * Adds a JSON value to the values vector.
     * 
     * @param value the value to add
     */
    void addValue(JSONValue *value);

    /**
     * Returns the values vector.
     * 
     * @return Vector of JSONValue pointers containing the values
     */
    std::vector<JSONValue *> getValues() const;

private:
    std::vector<JSONValue *> values;
};

#endif