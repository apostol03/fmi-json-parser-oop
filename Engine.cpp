#include "Engine.h"

Engine::Engine() {}

void Engine::prompt()
{
    if (!fileLoaded)
    {
        std::cout << "Please enter the path of the json file you wish to manipulate." << std::endl;
        std::string filePath;
        std::getline(std::cin, filePath);
        openFile(filePath);
    }

    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Welcome! Pick any of the following commands to operate with the JSON Parser: " << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "open <path> | validate | print | search <key> | " << std::endl;
    std::cout << "contains <value> | set <path> <string> | create <path> <string> | " << std::endl;
    std::cout << "delete <path> | move <from> <to> | save [<path>] | saveas <file> [<path>]" << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;

    std::string command;
    while (true)
    {
        std::cout << "[" << currentFilePath << "] " << "> ";
        std::getline(std::cin, command);
        if (command == "exit")
        {
            break;
        }
        executeCommand(command);
    }
}

void Engine::executeCommand(const std::string &command)
{
    if (command == "validate")
    {
        std::cout << (parser->validate() ? "Valid JSON file." : "Invalid JSON file.") << std::endl;
    }
    else if (command == "print")
    {
        // parser->print();   
    }
    else if (command.rfind("search ", 0) == 0)
    {
        std::string key = command.substr(7);
        std::vector<JSONValue*> values = parser->searchKey(key);
        std::cout << "\"" << key << "\"" << ":" << std::endl;
        std::cout << "[" << std::endl;
        for (const auto &value : values)
        {
            std::cout << "  " << value->toString() << std::endl;
        }
        std::cout << "]" << std::endl;
    }
    
}

void Engine::openFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "File does not exist. Creating a new empty file..." << std::endl;
        std::ofstream newFile(filePath);
        newFile << "{}";
        newFile.close();
        file.open(filePath);
        if (!file.is_open())
        {
            std::cout << "Could not create the file!" << std::endl;
            return;
        }
    }

    std::string input((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    try
    {
        parser = new Parser(input);
        fileLoaded = true;
        currentFilePath = filePath;
        std::cout << "Successfully loaded file at " << filePath << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading file: " << e.what() << std::endl;
        fileLoaded = false;
        delete parser;
        parser = nullptr;
    }
}