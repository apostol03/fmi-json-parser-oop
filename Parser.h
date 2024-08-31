#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>

#include "Validator.h"
#include "Printer.h"
#include "Searcher.h"

#include "JSONNull.h"

/**
 * Responsible for parsing and manipulation of JSON.
 */
class Parser
{
public:
    /**
     * Creates a parser object by parsing string input into a JSONValue object.
     */
    Parser(const std::string &stringInput);

    ~Parser();

    /**
     * Validates the JSON input.
     *
     * @return true if the JSON string is valid
     */
    bool validate();

    /**
     * Parses the string input into a JSONValue object.
     *
     * @return The JSONValue object
     */
    JSONValue *parse();

    /**
     * Writes the root JSONValue into a file at a given file path.
     *
     * @param filePath the path to the JSON file
     */
    void writeToFile(const std::string &filePath);

    /**
     * Print the JSON from the root.
     */
    void print();

    /**
     * Returns all values at a given key.
     *
     * @param key key to search values for
     *
     * @return vector of all values with the given key
     */
    std::vector<JSONValue *> searchKey(const std::string &key);

    /**
     * Returns if a value is present in the root JSON.
     *
     * @param value value to check
     *
     * @return true if the value is present in the root.
     */
    bool contains(const std::string &value);

    /**
     * Sets a new value at a given JSON path.
     *
     * @param path JSON path to the target element
     * @param newValue the new value to set
     *
     * @return true if the operation is successful
     */
    bool set(const std::string &path, const std::string &newValue);

    /**
     * Creates a new key-value pair in the root JSON.
     *
     * @param path JSON path to the new element
     * @param newValue the new value to set
     *
     * @return true if the operation is successful
     */
    bool create(const std::string &path, const std::string &newValue);

    /**
     * Deletes a key-value pair at a given JSON path.
     *
     * @param path JSON path to the target element
     *
     * @return true if the operation is successful
     */
    bool deleteElement(const std::string &path);

    /**
     * Moves a JSON key-value pair from one JSON path to another.
     *
     * @param fromPath source JSON path
     * @param toPath destination JSON path
     *
     * @return true if the operation is successful
     */
    bool move(const std::string &fromPath, const std::string &toPath);

    /**
     * Saves the current state of the JSON.
     * Might save only a portion of the file by a given JSON path.
     *
     * @param currPath the currently loaded file's path
     * @param filePath the path to the file to save to (optional)
     * @param path JSON path to the element to save (optional)
     *
     * @return true if the operation is successful
     */
    bool save(const std::string &currPath, const std::string &filePath = "", const std::string &path = "");

    /**
     * Saves the current state of the JSON to a given file path. Creates the file if it does not exist.
     * Might save only a portion of the file by a given JSON path.
     *
     * @param currPath the currently loaded file's path
     * @param filePath the path to the file to save to
     * @param path JSON path to the element to save (optional)
     *
     * @return true if the operation is successful
     */
    bool saveas(const std::string &currPath, const std::string &newFilePath, const std::string &path = "");

private:
    bool containsHelper(JSONValue *jsonValue, const std::string &value);

    /**
     * Splits a given JSON path by '/'.
     *
     * @param path JSON path to split
     *
     * @return vector of strings, representing each token after split
     */
    std::vector<std::string> splitPath(const std::string &path);

    /**
     * Parses a value, used for an update of an older value.
     *
     * @param lexer a new Lexer object to handle it
     *
     * @return the new JSONValue
     */
    JSONValue *parseNewValue(Lexer &lexer);

    JSONValue *navigateToPath(JSONValue *current, const std::vector<std::string> &tokens);

    /**
     * Writes JSON into an output file.
     *
     * @param outFile the file to write JSON to
     * @param value the JSONValue used for source
     * @param indent indentation, starting from zero
     */
    void writeJSON(std::ostream &outFile, JSONValue *value, int indent = 0) const;

private:
    /**
     * Parses a JSON value.
     *
     * @return the parsed JSONValue
     */
    JSONValue *parseValue();

    /**
     * Parses a JSON object.
     *
     * @return the parsed JSONObject
     */
    JSONObject *parseObject();

    /**
     * Parses a JSON array.
     *
     * @return the parsed JSONArray
     */
    JSONArray *parseArray();

private:
    JSONValue *root;
    Lexer lexer;
};

#endif