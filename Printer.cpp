#include "Printer.h"

void Printer::print(const JSONValue *jsonValue, int indentLevel)
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

void Printer::printString(const JSONString *jsonString)
{
    std::cout << jsonString->toString();
}

void Printer::printNumber(const JSONNumber *jsonNumber)
{
    std::cout << jsonNumber->toString();
}

void Printer::printBool(const JSONBool *jsonBool)
{
    std::cout << jsonBool->toString();
}

void Printer::printObject(const JSONObject *jsonObject, int indentLevel)
{
    std::cout << "{\n";
    const auto &values = jsonObject->getValues();
    for (size_t i = 0; i < values.size(); ++i)
    {
        std::cout << std::string(indentLevel + 2, ' ') << "\"" << values[i].key << "\": ";
        print(values[i].value, indentLevel + 2);

        if (i < values.size() - 1)
        {
            std::cout << ",";
        }

        std::cout << "\n";
    }
    std::cout << std::string(indentLevel, ' ') << "}";
}

void Printer::printArray(const JSONArray *jsonArray, int indentLevel)
{
    std::string indent(indentLevel, ' ');
    std::cout << "[\n";
    const auto &values = jsonArray->getValues();
    for (size_t i = 0; i < values.size(); ++i)
    {
        std::cout << std::string(indentLevel + 2, ' ');
        print(values[i], indentLevel + 2);

        if (i < values.size() - 1)
        {
            std::cout << ",";
        }

        std::cout << "\n";
    }
    std::cout << std::string(indentLevel, ' ') << "]";
}
