#pragma once

#include "Token.h"
#include <vector>

class Lexer {
public:
    std::vector<Token> tokenize(const std::string& input);

    bool isWhitespace(char c);

    bool isDelimiter(char c);

    bool isComment(char c);

private:
    // Private methods for tokenizing
};
