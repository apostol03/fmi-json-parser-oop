#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <sstream>

#include "Lexer.h"
#include "Validator.h"
#include "Printer.h"

#include "JSONValue.h"
#include "JSONString.h"
#include "JSONNumber.h"
#include "JSONBool.h"
#include "JSONArray.h"
#include "JSONObject.h"
#include "JSONNull.h"
#include "Searcher.h"

class Parser
{
public:
    Parser(const std::string &stringInput) : lexer(stringInput)
    {
        root = parseValue();
    }

    ~Parser()
    {
        delete root;
    }

    bool validate()
    {
        Validator validator(lexer);
        return validator.validate();
    }

    JSONValue *parse()
    {
        if (!validate())
        {
            std::cerr << "Invalid JSON input!" << std::endl;
        }

        return parseValue();
    }

    void writeToFile(const std::string &filePath)
    {
        std::ofstream outFile(filePath);
        if (!outFile.is_open())
        {
            throw std::runtime_error("Could not open file to write.");
        }

        writeJSON(outFile, root, 0);
        outFile.close();
    }

    void print()
    {
        Printer::print(root);
    }

    std::vector<JSONValue *> searchKey(const std::string &key)
    {
        try
        {
            return Searcher::searchByKey(static_cast<JSONObject *>(root), key);
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << e.what() << '\n';
            throw;
        }
    }

    bool contains(const std::string &value)
    {
        return root && containsHelper(root, value);
    }

    bool set(const std::string &path, const std::string &newValue)
    {
        if (!root)
        {
            std::cerr << "No JSON structure parsed." << std::endl;
            return false;
        }

        std::vector<std::string> tokens = splitPath(path);
        if (tokens.empty())
        {
            std::cerr << "Invalid JSON path." << std::endl;
            return false;
        }

        JSONValue *target = navigateToPath(root, tokens);
        if (!target)
        {
            std::cerr << "Path not found: " << path << std::endl;
            return false;
        }

        Lexer valueLexer(newValue);
        JSONValue *parsedNewValue = parseNewValue(valueLexer);
        if (!parsedNewValue)
        {
            std::cerr << "Invalid new value: " << newValue << std::endl;
            return false;
        }

        target = parsedNewValue;
        delete parsedNewValue;

        return true;
    }

private:
    bool containsHelper(JSONValue *jsonValue, const std::string &value)
    {
        switch (jsonValue->getType())
        {
        case JSONValueType::STRING:
        {
            JSONString *jsonString = static_cast<JSONString *>(jsonValue);
            return jsonString->toString().find(value) != std::string::npos;
        }
        case JSONValueType::NUMBER:
        {
            JSONNumber *jsonNumber = static_cast<JSONNumber *>(jsonValue);
            return std::stod(jsonNumber->toString()) == std::stod(value);
        }
        case JSONValueType::BOOL:
        {
            JSONBool *jsonBool = static_cast<JSONBool *>(jsonValue);
            return (jsonBool->toString() == value);
        }
        case JSONValueType::OBJECT:
        {
            JSONObject *jsonObject = static_cast<JSONObject *>(jsonValue);
            for (const auto &keyValue : jsonObject->getValues())
            {
                if (containsHelper(keyValue.value, value))
                {
                    return true;
                }
            }
            break;
        }
        case JSONValueType::ARRAY:
        {
            JSONArray *jsonArray = static_cast<JSONArray *>(jsonValue);
            for (const auto &item : jsonArray->getValues())
            {
                if (containsHelper(item, value))
                {
                    return true;
                }
            }
            break;
        }
        case JSONValueType::NILL:
            return false;
        }

        return false;
    }

    JSONValue *navigateToPath(JSONValue *current, const std::vector<std::string> &tokens)
    {
        for (size_t i = 0; i < tokens.size(); i++)
        {
            if (current->getType() == JSONValueType::OBJECT)
            {
                JSONObject *obj = static_cast<JSONObject *>(current);
                auto keyValues = obj->getValues();
                bool found = false;

                for (auto &pair : keyValues)
                {
                    if (pair.key == tokens[i])
                    {
                        current = pair.value;
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    return nullptr;
                }
            }
            else if (current->getType() == JSONValueType::ARRAY)
            {
                JSONArray *arr = static_cast<JSONArray *>(current);
                size_t index = std::stoi(tokens[i]);
                if (index >= arr->getValues().size())
                {
                    return nullptr;
                }
                current = arr->getValues()[index];
            }
            else
            {
                return nullptr;
            }
        }
        return current;
    }

    std::vector<std::string> splitPath(const std::string &path)
    {
        std::vector<std::string> tokens;
        std::stringstream ss(path);
        std::string item;

        while (std::getline(ss, item, '/'))
        {
            if (!item.empty())
                tokens.push_back(item);
        }

        return tokens;
    }

    JSONValue *parseNewValue(Lexer &lexer)
    {
        Token token = lexer.nextToken();
        switch (token.type)
        {
        case TokenType::LEFT_BRACE:
            return parseObject();
        case TokenType::LEFT_BRACKET:
            return parseArray();
        case TokenType::STRING:
            return new JSONString(token.value);
        case TokenType::NUMBER:
            return new JSONNumber(std::stod(token.value));
        case TokenType::TRUE:
            return new JSONBool(true);
        case TokenType::FALSE:
            return new JSONBool(false);
        case TokenType::NULL_TYPE:
            return new JSONNull();
        default:
            throw std::runtime_error("Invalid value provided for setting.");
        }
    }

    void writeJSON(std::ostream &outFile, JSONValue *value, int indent = 0) const
    {
        std::string indentStr(indent, ' '); 

        switch (value->getType())
        {
        case JSONValueType::OBJECT:
        {
            outFile << "{\n";
            JSONObject *obj = static_cast<JSONObject *>(value);
            const auto &keyValues = obj->getValues();

            for (size_t i = 0; i < keyValues.size(); ++i)
            {
                outFile << indentStr << "  \"" << keyValues[i].key << "\": ";
                writeJSON(outFile, keyValues[i].value, indent + 2);
                if (i < keyValues.size() - 1)
                {
                    outFile << ",";
                }
                outFile << "\n";
            }

            outFile << indentStr << "}";
            break;
        }
        case JSONValueType::ARRAY:
        {
            outFile << "[\n";
            JSONArray *arr = static_cast<JSONArray *>(value);
            const auto &values = arr->getValues();

            for (size_t i = 0; i < values.size(); ++i)
            {
                outFile << indentStr << "  ";
                writeJSON(outFile, values[i], indent + 2);
                if (i < values.size() - 1)
                {
                    outFile << ",";
                }
                outFile << "\n";
            }

            outFile << indentStr << "]";
            break;
        }
        case JSONValueType::STRING:
        {
            JSONString *str = static_cast<JSONString *>(value);
            outFile << str->toString();
            break;
        }
        case JSONValueType::NUMBER:
        {
            JSONNumber *num = static_cast<JSONNumber *>(value);
            outFile << num->toString();
            break;
        }
        case JSONValueType::BOOL:
        {
            JSONBool *boolean = static_cast<JSONBool *>(value);
            outFile << boolean->toString();
            break;
        }
        case JSONValueType::NILL:
        {
            outFile << "null";
            break;
        }
        }
    }

private:
    JSONValue *parseValue()
    {
        Token token = lexer.nextToken();
        switch (token.type)
        {
        case TokenType::LEFT_BRACE:
            return parseObject();
        case TokenType::LEFT_BRACKET:
            return parseArray();
        case TokenType::STRING:
            return new JSONString(token.value);
        case TokenType::NUMBER:
            return new JSONNumber(std::stod(token.value));
        case TokenType::TRUE:
            return new JSONBool(true);
        case TokenType::FALSE:
            return new JSONBool(false);
        case TokenType::NULL_TYPE:
            return new JSONNull();
        default:
            throw std::runtime_error("Unexpected token: " + token.value);
        }
    }

    JSONObject *parseObject()
    {
        JSONObject *obj = new JSONObject();
        Token token;

        while (token.type != TokenType::RIGHT_BRACE)
        {
            token = lexer.nextToken();
            if (token.type != TokenType::STRING)
            {
                delete obj;
                throw std::runtime_error("Expected string key in object");
            }

            std::string key = token.value;
            token = lexer.nextToken();
            if (token.type != TokenType::COLON)
            {
                delete obj;
                throw std::runtime_error("Expected ':' after key in object");
            }

            JSONValue *value = parseValue();
            if (!value)
            {
                delete obj;
                return nullptr;
            }

            obj->addValue(key, value);
            token = lexer.nextToken();
            if (token.type != TokenType::COMMA && token.type != TokenType::RIGHT_BRACE)
            {
                delete obj;
                throw std::runtime_error("Expected ',' or '}' in object");
            }
        }

        return obj;
    }

    JSONArray *parseArray()
    {
        JSONArray *arr = new JSONArray();
        Token token;

        while (token.type != TokenType::RIGHT_BRACKET)
        {
            JSONValue *value = parseValue();
            if (!value)
            {
                delete arr;
                return nullptr;
            }
            arr->addValue(value);

            token = lexer.nextToken();
            if (token.type != TokenType::COMMA && token.type != TokenType::RIGHT_BRACKET)
            {
                delete arr;
                throw std::runtime_error("Expected ',' or ']' in array");
            }
        }

        return arr;
    }

private:
    JSONValue *root;
    Lexer lexer;
};

#endif