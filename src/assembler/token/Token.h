#pragma once

#include "../../utils/Enums.h"
#include <string>

class Token {
public:
    Token(TokenType type, const std::string& value, int lineNumber);

    TokenType getType() const;
    const std::string& getValue() const;
    int getLineNumber() const;

private:
    TokenType type;
    std::string value;
    int lineNumber;
};
