#pragma once

#include "Lexer.h"
#include "SymbolTable.h"
#include "Instruction.h"
#include "Token.h"

class Parser {
public:
    Instruction parse(const std::vector<Token>& tokens, SymbolTable& symbolTable);
private:
    void checkValidInstruction(const std::vector<Token> &tokens, InstructionType type, std::string& opcode);
};
