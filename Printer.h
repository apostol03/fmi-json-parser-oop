#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>

#include "JSONValue.h"
#include "JSONString.h"
#include "JSONNumber.h"
#include "JSONObject.h"
#include "JSONArray.h"
#include "JSONBool.h"

class Printer
{
public:
    void print(const JSONValue *jsonValue, int indentLevel = 0)
    {
        switch (jsonValue->getType())
        {
        case JSONValueType::STRING:
            printString(static_cast<const JSONString *>(jsonValue));
            break;
        case JSONValueType::NUMBER:
            printNumber(static_cast<const JSONNumber *>(jsonValue));
            break;
        case JSONValueType::BOOL:
            printBool(static_cast<const JSONBool *>(jsonValue));
            break;
        case JSONValueType::OBJECT:
            printObject(static_cast<const JSONObject *>(jsonValue), indentLevel);
            break;
        case JSONValueType::ARRAY:
            printArray(static_cast<const JSONArray *>(jsonValue), indentLevel);
            break;
        case JSONValueType::NILL:
            std::cout << "null";
            break;
        }
    }

private:
    void printString(const JSONString *jsonString)
    {
        std::cout << jsonString->toString();
    }

    void printNumber(const JSONNumber *jsonNumber)
    {
        std::cout << jsonNumber->toString();
    }

    void printBool(const JSONBool *jsonBool)
    {
        std::cout << jsonBool->toString();
    }

    void printObject(const JSONObject *jsonObject, int indentLevel)
    {
        std::string indent(indentLevel, ' ');
        std::cout << "{\n";
        const auto &values = jsonObject->getValues();
        for (size_t i = 0; i < values.size(); ++i)
        {
            std::cout << indent << "\"" << values[i].key << "\": ";
            print(values[i].value, indentLevel + 2);

            if (i < values.size() - 1)
            {
                std::cout << ",";
            }

            std::cout << "\n";
        }
        std::cout << indent << "}";
    }

    void printArray(const JSONArray *jsonArray, int indentLevel)
    {
        std::string indent(indentLevel, ' ');
        std::cout << "[\n";
        const auto &values = jsonArray->getValues();
        for (size_t i = 0; i < values.size(); ++i)
        {
            std::cout << indent;
            print(values[i], indentLevel + 2);

            if (i < values.size() - 1)
            {
                std::cout << ",";
            }

            std::cout << "\n";
        }
        std::cout << indent << "]";
    }
};

#endif