#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>

#include "JSONValue.h"
#include "JSONString.h"
#include "JSONNumber.h"
#include "JSONObject.h"
#include "JSONArray.h"
#include "JSONBool.h"

/**
 * Class used for printing operations.
 */
class Printer
{
public:
    /**
     * Pretty-prints a JSON value.
     *
     * @param jsonValue the JSON value to print
     * @param indentLevel indentation level for pretty-printing (starts at 0)
     */
    static void print(const JSONValue *jsonValue, int indentLevel = 0);

private:
    /**
     * Prints a JSON string.
     *
     * @param jsonString the JSON string to print
     */
    static void printString(const JSONString *jsonString);

    /**
     * Prints a JSON number.
     *
     * @param jsonNumber the JSON number to print
     */
    static void printNumber(const JSONNumber *jsonNumber);

    /**
     * Prints a JSON boolean.
     *
     * @param jsonBool the JSON boolean to print
     */
    static void printBool(const JSONBool *jsonBool);

    /**
     * Pretty-prints a JSON object.
     *
     * @param jsonObject the JSON object to print
     * @param indentLevel indentation level for pretty-printing
     */
    static void printObject(const JSONObject *jsonObject, int indentLevel);

    /**
     * Prints a JSON string.
     *
     * @param jsonArray the JSON array to print
     * @param indentLevel indentation level for pretty-printing
     */
    static void printArray(const JSONArray *jsonArray, int indentLevel);
};

#endif