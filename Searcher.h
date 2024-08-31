#ifndef SEARCHER_H
#define SEARCHER_H

#include "JSONObject.h"
#include "JSONArray.h"

/**
 * Provides searching functionality in a JSON value.
 */
class Searcher
{
public:
    /**
     * Retrieves all values in a JSON object with a given key.
     *
     * @param jsonObject the JSON object to search in
     * @param key the key to find values for
     *
     * @return Vector of JSONValue pointers, containing all values corresponding to the given key
     */
    static std::vector<JSONValue *> searchByKey(const JSONObject *jsonObject, const std::string &key);

private:
    /**
     * Searches for values with a given key in a JSON object and fills them in a vector.
     *
     * @param jsonObject the JSON object to search in
     * @param key the key to find values for
     * @param results vector of values to fill
     */
    static void searchObject(const JSONObject *jsonObject, const std::string &key, std::vector<JSONValue *> &results);
    
    /**
     * Searches for values with a given key in a JSON array and fills them in a vector.
     *
     * @param jsonArray the JSON array to search in
     * @param key the key to find values for
     * @param results vector of values to fill
     */
    static void searchArray(const JSONArray *jsonArray, const std::string &key, std::vector<JSONValue *> &results);
};

#endif