#ifndef PARSER_H
#define PARSER_H

#include <iostream>

#include "Lexer.h"
#include "Validator.h"

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

    std::vector<JSONValue*> searchKey(const std::string &key)
    {
        try
        {
            return Searcher::searchByKey(static_cast<JSONObject*>(root), key);
        }
        catch(const std::runtime_error& e)
        {
            std::cerr << e.what() << '\n';
            throw;
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
        
        do
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
            
        } while (token.type != TokenType::RIGHT_BRACE);
        
        return obj;
    }

    JSONArray *parseArray()
    {
        JSONArray *arr = new JSONArray();
        Token token;

        do {
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
        } while (token.type != TokenType::RIGHT_BRACKET);
        
        return arr;
    }

private:
    JSONValue *root;
    Lexer lexer;
};

#endif