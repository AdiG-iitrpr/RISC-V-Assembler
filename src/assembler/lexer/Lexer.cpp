#include "Lexer.h"
#include "../../utils/NumberUtils.h"

#include <iostream>
#include <ctype.h>
#include <cctype>
#include <algorithm>

std::vector<Token> Lexer::tokenize(const std::string& input, SymbolTable &symbolTable) {
    std::vector<Token> tokens;
    std::string currentToken;
    std::string currentLabel;
    std::vector<std::string> toUpdateLabels;

    int lineNumber = 1;
    bool instruction = false;

    for (std::size_t i = 0; i < input.length(); ++i) {
        char c = input[i];

        if (isWhitespace(c) || isDelimiter(c)) {

            if (!currentToken.empty()) {
                currentToken = mapSpecialRegisters(currentToken);
                TokenType tokenType = getTokenType(currentToken);

                if (tokenType == TokenType::LABEL) {
                    currentLabel = currentToken;
                }

                if (tokenType == TokenType::INSTRUCTION) {
                    instruction = true;
                    currentToken = toLowerCase(currentToken);
                    if (!toUpdateLabels.empty()) {
                        for (std::size_t j = 0; j < toUpdateLabels.size(); j++) {
                            if (symbolTable.getLabelInstructionLineNumber(toUpdateLabels[j]) == -1)
                                symbolTable.addLabel(toUpdateLabels[j], lineNumber);
                            else
                                throw std::runtime_error("Label " + toUpdateLabels[j] + " defined more than once");
                        }
                        toUpdateLabels.clear();
                    }
                }

                tokens.push_back(Token(tokenType, currentToken, lineNumber));

                currentToken.clear();
            }

            if (c == ':') {
                toUpdateLabels.push_back(currentLabel);
            }


        } else if (isComment(c)) {
            while (i < input.length() && c != '\n' && c != '\r') {
                c = input[++i];
            }
        } else {
            currentToken += c;
        }

        if (c == '\n' and instruction)
            lineNumber++;

        if (c == '\n')
            instruction = false;
    }


    if (!currentToken.empty()) {
        currentToken = mapSpecialRegisters(currentToken);
        TokenType tokenType = getTokenType(currentToken);

        if (tokenType == TokenType::LABEL) {
            currentLabel = currentToken;
        }

        if (tokenType == TokenType::INSTRUCTION) {
            currentToken = toLowerCase(currentToken);
            if (!toUpdateLabels.empty()) {
                for (std::size_t j = 0; j < toUpdateLabels.size(); j++) {
                    if (symbolTable.getLabelInstructionLineNumber(toUpdateLabels[j]) == -1)
                        symbolTable.addLabel(toUpdateLabels[j], lineNumber);
                    else
                        throw std::runtime_error("Label " + toUpdateLabels[j] + " defined more than once");
                }
                toUpdateLabels.clear();
            }
        }

        tokens.push_back(Token(tokenType, currentToken, lineNumber));
    }

    if (!toUpdateLabels.empty()) {
        for (std::size_t j = 0; j < toUpdateLabels.size(); j++) {
            if (symbolTable.getLabelInstructionLineNumber(toUpdateLabels[j]) == -1)
                symbolTable.addLabel(toUpdateLabels[j], lineNumber);
            else
                throw std::runtime_error("Label " + toUpdateLabels[j] + " defined more than once");
        }
        toUpdateLabels.clear();
    }

    return tokens;
}

bool Lexer::isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == ':';
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

DirectiveType Lexer::getDirectiveType(const std::string& directive) {

    if (directive == ".text")
        return DirectiveType::TEXT;
    if (directive == ".data")
        return DirectiveType::DATA;
    if (directive == ".byte")
        return DirectiveType::BYTE;
    if (directive == ".half")
        return DirectiveType::HALF;
    if (directive == ".word")
        return DirectiveType::WORD;
    if (directive == ".dword")
        return DirectiveType::DWORD;
    if (directive == ".asciiz")
        return DirectiveType::ASCIIZ;

    return DirectiveType::UNKNOWN;
}

TokenType Lexer::getTokenType(const std::string& token) {

    if (token.front() == '"' && token.back() == '"') {
        return TokenType::IMMEDIATE;
    }

    // instructions not necessary to be in lower case

    auto it = std::find_if(instructionMap.begin(), instructionMap.end(),
    [token](const auto & entry) {
        return caseInsensitiveCompare(entry.first, token);
    });

    if (it != instructionMap.end()) {
        return TokenType::INSTRUCTION;
    }

    // Check if the token is a register
    if (token.size() > 1 && token.front() == 'x' &&
            std::all_of(token.begin() + 1, token.end(), ::isdigit) &&
            std::stoi(token.substr(1)) >= 0 && std::stoi(token.substr(1)) <= 31) {
        return TokenType::REGISTER;
    }

    // Check if the token is an immediate (constant value)
    if (token.front() == '-' && std::isdigit(token[1])) {
        return TokenType::IMMEDIATE;
    } else if (std::isdigit(token.front())) {
        return TokenType::IMMEDIATE;
    } else if (token.size() > 2 && token.substr(0, 2) == "0x" && NumberUtils::isHexadecimal(token.substr(2))) {
        return TokenType::IMMEDIATE;
    } else if (token.size() > 3 && token.substr(0, 3) == "-0x" && NumberUtils::isHexadecimal(token.substr(3))) {
        return TokenType::IMMEDIATE;
    } else if (token.size() > 2 && token.substr(0, 2) == "0b" && NumberUtils::isBinary(token.substr(2))) {
        return TokenType::IMMEDIATE;
    } else if (token.size() > 3 && token.substr(0, 2) == "-0b" && NumberUtils::isBinary(token.substr(3))) {
        return TokenType::IMMEDIATE;
    }

    // Check if token is a directive
    DirectiveType directiveType = getDirectiveType(token);
    if (directiveType != DirectiveType::UNKNOWN) {
        return TokenType::DIRECTIVE;
    }

    return TokenType::LABEL;
}