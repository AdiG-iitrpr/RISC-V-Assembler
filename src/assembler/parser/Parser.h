#pragma once

#include "../lexer/Lexer.h"
#include "../labelTable/SymbolTable.h"
#include "../instruction/Instruction.h"
#include "../token/Token.h"

class Parser {
public:
    Instruction parse(const std::vector<Token>& tokens, SymbolTable& symbolTable);
private:
    void checkValidInstruction(const std::vector<Token> &tokens, InstructionType type, std::string& opcode);
};
