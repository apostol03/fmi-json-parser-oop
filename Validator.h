#include "Lexer.h"

class Validator
{
public:
    Validator(Lexer &lexer) : lexer(lexer), currentToken(lexer.nextToken()) {}

    bool validate()
    {
        try
        {
            lexer.resetPos();
            currentToken = lexer.nextToken();
            validateValue();
            if (currentToken.type != TokenType::END)
            {
                throw std::runtime_error("Unexpected characters at the end of JSON input.");
            }
            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Validation error: " << e.what() << std::endl;
            return false;
        }
    }

private:
    void validateValue()
    {
        switch (currentToken.type)
        {
        case TokenType::LEFT_BRACE:
            validateObject();
            break;
        case TokenType::LEFT_BRACKET:
            validateArray();
            break;
        case TokenType::STRING:
            validateString();
            break;
        case TokenType::NUMBER:
            validateNumber();
            break;
        case TokenType::TRUE:
        case TokenType::FALSE:
        case TokenType::NULL_TYPE:
            currentToken = lexer.nextToken();
            break;
        default:
            throw std::runtime_error("Unexpected token at line " + std::to_string(lexer.getLine()) + ", column " + std::to_string(lexer.getColumn()));
        }
    }

    void validateObject()
    {
        currentToken = lexer.nextToken();
        if (currentToken.type != TokenType::RIGHT_BRACE)
        {
            while (true)
            {
                if (currentToken.type != TokenType::STRING)
                {
                    throwError("Expected string");
                }
                validateString();
                if (currentToken.type != TokenType::COLON)
                {
                    throwError("Expected ':'");
                }
                currentToken = lexer.nextToken();
                validateValue();
                if (currentToken.type == TokenType::COMMA)
                {
                    currentToken = lexer.nextToken();
                }
                else
                {
                    break;
                }
            }
            if (currentToken.type != TokenType::RIGHT_BRACE)
                throwError("Expected '}'");
        }
        currentToken = lexer.nextToken();
    }

    void validateArray()
    {
        currentToken = lexer.nextToken();
        if (currentToken.type != TokenType::RIGHT_BRACKET)
        {
            while (true)
            {
                validateValue();
                if (currentToken.type == TokenType::COMMA)
                {
                    currentToken = lexer.nextToken();
                }
                else
                {
                    break;
                }
            }
            if (currentToken.type != TokenType::RIGHT_BRACKET)
            {
                throwError("Expected ']'");
            }
        }
        currentToken = lexer.nextToken();
    }

    void validateString()
    {
        if (currentToken.type != TokenType::STRING)
        {
            throwError("Expected string");
        }
        currentToken = lexer.nextToken();
    }

    void validateNumber()
    {
        if (currentToken.type != TokenType::NUMBER)
        {
            throwError("Expected number");
        }
        currentToken = lexer.nextToken();
    }

    void throwError(const std::string &message)
    {
        throw std::runtime_error(message + " at line " + std::to_string(lexer.getLine()) + ", column " + std::to_string(lexer.getColumn()));
    }

private:
    Lexer lexer;
    Token currentToken;
};