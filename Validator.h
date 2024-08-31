#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "Lexer.h"

/**
 * Class used for validating JSON.
 */
class Validator
{
public:
    /**
     * Constructor of a validator object with a given Lexer object.
     */
    Validator(Lexer &lexer);

    /**
     * Validates the JSON string provided in the Lexer object.
     *
     * @return true if the JSON string is a valid JSON format
     */
    bool validate();

private:
    /**
     * Validates a JSON value.
     *
     * @throws {std::runtime_error} if there is incorrect JSON in the value
     */
    void validateValue();

    /**
     * Validates a JSON object.
     *
     * @throws {std::runtime_error} if there is incorrect JSON
     */
    void validateObject();

    /**
     * Validates a JSON array.
     *
     * @throws {std::runtime_error} if there is incorrect JSON
     */
    void validateArray();

    /**
     * Validates a JSON string.
     * 
     * @throws {std::runtime_error} if there is incorrect JSON
     */
    void validateString();

    /**
     * Validates a JSON value.
     * 
     * @throws {std::runtime_error} if there is incorrect JSON
     */
    void validateNumber();

    /**
     * Throws a std::runtime_error with a message.
     * 
     * @param message message to display in the error
     */
    void throwError(const std::string &message);

private:
    Lexer lexer;
    Token currentToken;
};

#endif