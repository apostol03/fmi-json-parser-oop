#ifndef LEXER_H
#define LEXER_H

#pragma once

#include <iostream>
#include <string>

/**
 * Enum representing the type of a token in JSON.
 */
enum class TokenType
{
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    COMMA,
    COLON,
    STRING,
    NUMBER,
    TRUE,
    FALSE,
    NULL_TYPE,
    END
};

/**
 * Structure representing a token in JSON.
 */
struct Token
{
    TokenType type;
    std::string value;

    Token(TokenType type = TokenType::END, const std::string &value = "")
        : type(type), value(value) {}
};

/**
 * Class responsible for lexical analysis of JSON input.
 */
class Lexer
{
public:
    /**
     * Constructs a Lexer object with the given input.
     * @param input JSON input string.
     */
    Lexer(const std::string &input);

    /**
     * Gets the current line number.
     * @return Current line number.
     */
    size_t getLine();

    /**
     * Gets the current column number.
     * @return Current column number.
     */
    size_t getColumn();

    /**
     * Gets the next token from the input.
     * @return The next token.
     */
    Token nextToken();

    /**
     * Resets the position to the beginning of the input.
     */
    void resetPos();

private:
    /**
     * Advances the current position by one character.
     */
    void advance();

    /**
     * Skips whitespace characters in the input.
     */
    void skipWhitespace();

    /**
     * Parses a string token.
     * @return The parsed string token.
     */
    Token parseString();

    /**
     * Parses a number token.
     * @return The parsed number token.
     */
    Token parseNumber();

    /**
     * Parses a keyword token (true, false, null).
     * @return The parsed keyword token.
     */
    Token parseKeyword();

private:
    std::string input;
    size_t pos;
    size_t line;
    size_t column;
};

#endif
