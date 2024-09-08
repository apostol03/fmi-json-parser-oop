#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

#include "JSONValue.h"
#include <iostream>

/**
 * Structure representing a key-value pair where the key
 * is a std::string and the value is a JSONValue pointer.
 */
struct KeyValue
{
    std::string key;
    JSONValue *value;

    KeyValue(const std::string &key, JSONValue *value) : key(key), value(value) {}
};

class JSONObject : public JSONValue
{
public:
    JSONValueType getType() const override;
    std::string toString() const override;

    /**
     * Returns the values vector.
     *
     * @return Vector of KeyValue pairs containing the values
     */
    std::vector<KeyValue> getValues() const;

    /**
     * Returns the value at a given key.
     *
     * @param key the key to find the value by
     * @return JSONValue pointer to the value at the given key
     */
    JSONValue *getValue(const std::string &key);

    /**
     * Updates the value at a given key.
     *
     * @param key the key of the value to update
     * @param newValue the value to replace the old one with
     */
    void setValue(const std::string &key, JSONValue *newValue);

    /**
     * Adds a new pair of key and value to the values vector.
     *
     * @param key
     * @param value
     */
    void addValue(const std::string &key, JSONValue *value);

    /**
     * Removes a value from the values vector.
     * 
     * @param key used to find the value to remove
     */
    void removeValue(const std::string &key);

private:
    std::vector<KeyValue> values;
};

#endif