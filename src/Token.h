#include <string>

enum class TokenType {
    INSTRUCTION,
    REGISTER,
    LABEL,
    DIRECTIVE,
    IMMEDIATE,
    END_OF_LINE,
    INVALID
};

class Token {
public:
    Token(TokenType type, const std::string& value);

    TokenType getType() const;
    const std::string& getValue() const;

private:
    TokenType type;
    std::string value;
};

