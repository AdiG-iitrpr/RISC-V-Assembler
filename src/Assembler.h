#pragma once

#include "Lexer.h"
#include "Parser.h"
#include "SymbolTable.h"
#include "Token.h"
#include "Instruction.h"

#include <string>
#include <vector>
#include <bitset>

class Assembler {
public:
    Assembler(Lexer &lexer, Parser &parser, SymbolTable &symbolTable);
    virtual ~Assembler();

    void assemble(const std::string& inputFilePath, const std::string& outputFilePath);

private:
    Lexer &lexer;
    Parser &parser;
    SymbolTable &symbolTable;

    uint32_t codeSegmentAddress;
    uint32_t dataSegmentAddress;

    void handleDirective(const std::string& directive, const std::vector<Token>& tokens);
    std::string readFile(const std::string& filePath);
    int binaryStringToNumber(const std::string& binaryString);
    std::string binaryToHex(const std::bitset<32>&bits);
    std::string decimalToHex(uint32_t decimal);
    int convertImmediateToInteger(const std::string& imm);
    std::bitset<32> generateMachineCode(const Instruction& instruction);
    std::bitset<32> generateRTypeMachineCode(const std::string& opcode, const std::string& funct3, const std::string& funct7, const std::vector<std::string>& operands);
    std::bitset<32> generateITypeMachineCode(const std::string& opcode, const std::string& funct3, const std::vector<std::string>& operands);
    std::bitset<32> generateSTypeMachineCode(const std::string& opcode, const std::string& funct3, const std::vector<std::string>& operands);
    std::bitset<32> generateSBTypeMachineCode(const std::string& opcode, const std::string& funct3, const std::vector<std::string>& operands);
    std::bitset<32> generateUTypeMachineCode(const std::string& opcode, const std::vector<std::string>& operands);
    std::bitset<32> generateUJTypeMachineCode(const std::string& opcode, const std::vector<std::string>& operands);
};
