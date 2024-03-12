#include "Token.h"

Token::Token(TokenType type, const std::string& value, int lineNumber) : type(type), value(value), lineNumber(lineNumber) {}

TokenType Token::getType() const {
    return type;
}

const std::string& Token::getValue() const {
    return value;
}

int Token::getLineNumber() const {
    return lineNumber;
}
