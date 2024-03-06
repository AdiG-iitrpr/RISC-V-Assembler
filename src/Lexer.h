#pragma once

#include "Token.h"
#include "RegisterAliases.h"
#include <vector>
#include <string>
#include <unordered_map>

class Lexer {
public:
    std::vector<Token> tokenize(const std::string& input);

private:

    const std::unordered_map<std::string, std::string>& registerAliasMap = RegisterAliases::registerAliasMap;
    bool isWhitespace(char c);
    bool isDelimiter(char c);
    bool isComment(char c);
    std::string mapSpecialRegisters(const std::string& alias);
};
