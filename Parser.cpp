#include "Parser.h"

Parser::Parser(const std::string &stringInput) : lexer(stringInput)
{
    root = parseValue();
}

Parser::~Parser()
{
    delete root;
}

bool Parser::validate()
{
    Validator validator(lexer);
    return validator.validate();
}

JSONValue *Parser::parse()
{
    if (!validate())
    {
        std::cerr << "Invalid JSON input!" << std::endl;
    }

    return parseValue();
}

void Parser::writeToFile(const std::string &filePath)
{
    std::ofstream outFile(filePath);
    if (!outFile.is_open())
    {
        throw std::runtime_error("Could not open file to write.");
    }
    print();
    writeJSON(outFile, root, 0);
    outFile.close();
}

void Parser::print()
{
    Printer::print(root);
}

std::vector<JSONValue *> Parser::searchKey(const std::string &key)
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

bool Parser::contains(const std::string &value)
{
    return root && containsHelper(root, value);
}

bool Parser::set(const std::string &path, const std::string &newValue)
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

    JSONValue *parent = root;
    std::string lastToken = tokens.back();
    tokens.pop_back();

    for (const std::string &token : tokens)
    {
        JSONObject *obj = dynamic_cast<JSONObject *>(parent);
        if (!obj)
        {
            std::cerr << "Invalid path: " << path << std::endl;
            return false;
        }
        parent = obj->getValue(token);
    }

    JSONObject *parentObj = dynamic_cast<JSONObject *>(parent);
    if (!parentObj)
    {
        std::cerr << "Invalid path: " << path << std::endl;
        return false;
    }

    JSONValue *target = parentObj->getValue(lastToken);
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

    parentObj->setValue(lastToken, parsedNewValue);

    return true;
}

bool Parser::create(const std::string &path, const std::string &newValue)
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

    JSONValue *parent = root;
    std::string lastToken = tokens.back();
    tokens.pop_back();

    for (const std::string &token : tokens)
    {
        JSONObject *obj = dynamic_cast<JSONObject *>(parent);
        if (!obj)
        {
            std::cerr << "Invalid path: " << path << std::endl;
            return false;
        }
        JSONValue *nextValue = obj->getValue(token);
        if (!nextValue)
        {
            nextValue = new JSONObject();
            obj->setValue(token, nextValue);
        }
        parent = nextValue;
    }

    JSONObject *parentObj = dynamic_cast<JSONObject *>(parent);
    if (!parentObj)
    {
        std::cerr << "Invalid path: " << path << std::endl;
        return false;
    }

    if (parentObj->getValue(lastToken))
    {
        std::cerr << "Element already exists at path: " << path << std::endl;
        return false;
    }

    Lexer valueLexer(newValue);
    JSONValue *parsedNewValue = parseNewValue(valueLexer);
    if (!parsedNewValue)
    {
        std::cerr << "Invalid new value: " << newValue << std::endl;
        return false;
    }

    parentObj->setValue(lastToken, parsedNewValue);

    return true;
}

bool Parser::deleteElement(const std::string &path)
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

    JSONValue *parent = root;
    std::string lastToken = tokens.back();
    tokens.pop_back();

    for (const std::string &token : tokens)
    {
        JSONObject *obj = dynamic_cast<JSONObject *>(parent);
        if (!obj)
        {
            std::cerr << "Invalid path: " << path << std::endl;
            return false;
        }
        JSONValue *nextValue = obj->getValue(token);
        if (!nextValue)
        {
            std::cerr << "Path not found: " << path << std::endl;
            return false;
        }
        parent = nextValue;
    }

    JSONObject *parentObj = dynamic_cast<JSONObject *>(parent);
    if (!parentObj)
    {
        std::cerr << "Invalid path: " << path << std::endl;
        return false;
    }

    if (!parentObj->getValue(lastToken))
    {
        std::cerr << "Element not found at path: " << path << std::endl;
        return false;
    }

    parentObj->removeValue(lastToken);
    return true;
}

// TODO: FIX
bool Parser::move(const std::string &fromPath, const std::string &toPath)
{
    if (!root)
    {
        std::cerr << "No JSON structure parsed." << std::endl;
        return false;
    }

    std::vector<std::string> fromTokens = splitPath(fromPath);
    JSONValue *parentFrom = root;
    std::string lastFromToken = fromTokens.back();
    fromTokens.pop_back();

    for (const std::string &token : fromTokens)
    {
        JSONObject *obj = dynamic_cast<JSONObject *>(parentFrom);
        if (!obj)
        {
            std::cerr << "Invalid 'from' path: " << fromPath << std::endl;
            return false;
        }
        parentFrom = obj->getValue(token);
        if (!parentFrom)
        {
            std::cerr << "Path not found: " << fromPath << std::endl;
            return false;
        }
    }

    JSONObject *parentFromObj = dynamic_cast<JSONObject *>(parentFrom);
    if (!parentFromObj)
    {
        std::cerr << "Invalid 'from' path: " << fromPath << std::endl;
        return false;
    }

    JSONValue *valueToMove = parentFromObj->getValue(lastFromToken);
    if (!valueToMove)
    {
        std::cerr << "Element not found at 'from' path: " << fromPath << std::endl;
        return false;
    }

    std::vector<std::string> toTokens = splitPath(toPath);
    JSONValue *parentTo = root;
    std::string lastToToken = toTokens.back();
    toTokens.pop_back();

    for (const std::string &token : toTokens)
    {
        JSONObject *obj = dynamic_cast<JSONObject *>(parentTo);
        if (!obj)
        {
            JSONObject *newObj = new JSONObject();
            dynamic_cast<JSONObject *>(parentTo)->addValue(token, newObj);
            parentTo = newObj;
        }
        else
        {
            parentTo = obj->getValue(token);
            if (!parentTo)
            {
                JSONObject *newObj = new JSONObject();
                obj->addValue(token, newObj);
                parentTo = newObj;
            }
        }
    }

    JSONObject *parentToObj = dynamic_cast<JSONObject *>(parentTo);
    if (!parentToObj)
    {
        std::cerr << "Invalid 'to' path: " << toPath << std::endl;
        return false;
    }

    parentToObj->setValue(lastToToken, valueToMove);
    parentFromObj->removeValue(lastFromToken);

    std::cerr << "Successfully moved value from " << fromPath << " to " << toPath << std::endl;

    return true;
}

bool Parser::save(const std::string &currPath, const std::string &filePath, const std::string &path)
{
    if (!root)
    {
        std::cerr << "No JSON structure parsed." << std::endl;
        return false;
    }

    std::string targetFile = filePath.empty() ? currPath : filePath;

    if (targetFile.empty())
    {
        std::cerr << "No file path specified." << std::endl;
        return false;
    }

    JSONValue *target = root;
    if (!path.empty())
    {
        std::vector<std::string> tokens = splitPath(path);
        target = navigateToPath(root, tokens);
        if (!target)
        {
            std::cerr << "Path not found: " << path << std::endl;
            return false;
        }
    }

    std::ofstream outFile(targetFile);
    if (!outFile.is_open())
    {
        std::cerr << "Could not open file to write: " << targetFile << std::endl;
        return false;
    }

    writeJSON(outFile, target, 0);
    outFile.close();

    return true;
}

bool Parser::saveas(const std::string &currPath, const std::string &newFilePath, const std::string &path)
{
    return save(currPath, newFilePath, path);
}

bool Parser::containsHelper(JSONValue *jsonValue, const std::string &value)
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
        return jsonNumber->toString() == value;
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

JSONValue *Parser::navigateToPath(JSONValue *current, const std::vector<std::string> &tokens)
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

std::vector<std::string> Parser::splitPath(const std::string &path)
{
    std::vector<std::string> tokens;
    std::stringstream ss(path);
    std::string item;

    while (std::getline(ss, item, '/'))
    {
        if (!item.empty())
        {
            tokens.push_back(item);
        }
    }

    return tokens;
}

JSONValue *Parser::parseNewValue(Lexer &lexer)
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

void Parser::writeJSON(std::ostream &outFile, JSONValue *value, int indent) const
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

JSONValue *Parser::parseValue()
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

JSONObject *Parser::parseObject()
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

JSONArray *Parser::parseArray()
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
