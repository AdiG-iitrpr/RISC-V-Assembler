#include "Lexer.h"

#include <iostream>
#include <ctype.h>
#include <cctype>
#include <algorithm>

std::vector<Token> Lexer::tokenize(const std::string& input, SymbolTable &symbolTable) {
    std::vector<Token> tokens;
    std::string currentToken;
    std::string currentLabel;

    int lineNumber = 1;

    for (std::size_t i = 0; i < input.length(); ++i) {
        char c = input[i];
        bool comment = false;
        bool label = false;

        if (isWhitespace(c) || isDelimiter(c)) {

            if (!currentToken.empty()) {
                currentToken = mapSpecialRegisters(currentToken);
                TokenType tokenType = getTokenType(currentToken);
                tokens.push_back(Token(tokenType, currentToken, lineNumber));

                std::cout << currentToken << std::endl;
                std::cout << lineNumber << std::endl;

                if (tokenType == TokenType::LABEL and currentToken[currentToken.size() - 1] == ':') {
                    label = true;
                    currentLabel = currentToken.substr(0, currentToken.size() - 1);
                }

                if (tokenType == TokenType::INSTRUCTION and !currentLabel.empty()) {
                    symbolTable.addLabel(currentLabel, lineNumber);
                    currentLabel.clear();
                }

                currentToken.clear();
            }
        } else if (isComment(c)) {
            comment = true;
            while (i < input.length() && c != '\n' && c != '\r') {
                c = input[++i];
            }
        } else {
            currentToken += c;
        }

        if (c == '\n' and !comment and !label)
            lineNumber++;
    }


    if (!currentToken.empty()) {
        currentToken = mapSpecialRegisters(currentToken);
        TokenType tokenType = getTokenType(currentToken);
        tokens.push_back(Token(tokenType, currentToken, lineNumber));

        std::cout << currentToken << std::endl;
        std::cout << lineNumber << std::endl;

        if (tokenType == TokenType::LABEL and currentToken[currentToken.size() - 1] == ':') {
            currentLabel = currentToken.substr(0, currentToken.size() - 1);
        }

        if (tokenType == TokenType::INSTRUCTION and !currentLabel.empty()) {
            symbolTable.addLabel(currentLabel, lineNumber);
            currentLabel.clear();
        }

    }

    if (!currentLabel.empty()) {
        symbolTable.addLabel(currentLabel, lineNumber);
        currentLabel.clear();
    }

    return tokens;
}

bool Lexer::isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool Lexer::isDelimiter(char c) {
    return c == ',' || c == '(' || c == ')';
}

bool Lexer::isComment(char c) {
    return c == '#';
}

std::string Lexer::mapSpecialRegisters(const std::string& alias) {
    auto it = registerAliasMap.find(alias);
    if (it != registerAliasMap.end()) {
        return it->second;
    }
    return alias;
}

TokenType Lexer::getTokenType(const std::string& token) {

    if (instructionMap.find(token) != instructionMap.end()) {
        return TokenType::INSTRUCTION;
    }

    if (token == ".text" || token == ".data" || token == ".byte" || token == ".half" || token == ".word" || token == ".dword" || token == ".asciiz") {
        return TokenType::DIRECTIVE;
    }

    // Check if the token is a register
    if (token.size() > 1 && token.front() == 'x' &&
            std::all_of(token.begin() + 1, token.end(), ::isdigit) &&
            std::stoi(token.substr(1)) >= 0 && std::stoi(token.substr(1)) <= 31) {
        return TokenType::REGISTER;
    }

    // Check if the token is an immediate (constant value)
    if (token.front() == '"' && token.back() == '"') {
        return TokenType::IMMEDIATE;
    }

    if (token.front() == '-' && std::isdigit(token[1])) {
        return TokenType::IMMEDIATE;
    } else if (std::isdigit(token.front())) {
        return TokenType::IMMEDIATE;
    } else if (token.size() > 2 && token.substr(0, 2) == "0x" && isHexadecimal(token.substr(2))) {
        return TokenType::IMMEDIATE;
    } else if (token.size() > 3 && token.substr(0, 3) == "-0x" && isHexadecimal(token.substr(3))) {
        return TokenType::IMMEDIATE;
    } else if (token.size() > 2 && token.substr(0, 2) == "0b" && isBinary(token.substr(2))) {
        return TokenType::IMMEDIATE;
    } else if (token.size() > 3 && token.substr(0, 2) == "-0b" && isBinary(token.substr(3))) {
        return TokenType::IMMEDIATE;
    }

    return TokenType::LABEL;
}

bool Lexer::isHexadecimal(const std::string& str) {
    for (char c : str) {
        if (!std::isxdigit(c)) {
            return false;
        }
    }
    return true;
}

bool Lexer::isBinary(const std::string& str) {
    for (char c : str) {
        if (c != '0' && c != '1') {
            return false;
        }
    }
    return true;
}