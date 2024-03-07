#pragma once

#include <string>

enum class TokenType {
    INSTRUCTION,
    REGISTER,
    LABEL,
    DIRECTIVE,
    IMMEDIATE,
    INVALID
};

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
