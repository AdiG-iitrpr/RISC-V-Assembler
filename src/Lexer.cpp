#include "Lexer.h"
#include<iostream>

std::vector<Token> Lexer::tokenize(const std::string& input) {
    std::vector<Token> tokens;
    std::string currentToken;

    for (std::size_t i = 0; i < input.length(); ++i) {
        char c = input[i];

        if (isWhitespace(c) || isDelimiter(c)) {

            if (!currentToken.empty()) {
                currentToken = mapSpecialRegisters(currentToken);
                tokens.push_back(Token(TokenType::INSTRUCTION, currentToken));
                std::cout << currentToken << std::endl;
                currentToken.clear();
            }
        } else if (isComment(c)) {
            // Skip the rest of the line if a comment is encountered
            while (i < input.length() && c != '\n' && c != '\r') {
                c = input[++i];
            }
        } else {
            currentToken += c;
        }
    }


    if (!currentToken.empty()) {
        currentToken = mapSpecialRegisters(currentToken);
        tokens.push_back(Token(TokenType::INSTRUCTION, currentToken));
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