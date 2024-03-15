#pragma once

#include "lexer/Lexer.h"
#include "parser/Parser.h"

#include <string>
#include <vector>
#include <bitset>
#include <fstream>

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

    void handleDirective(const std::string& directive, const std::vector<Token>& tokens, const size_t& tokenId, std::string& dataSegmentBuffer);
    std::string convertImmediateToHexadecimalFormat(const std::string& immed, const int& size);
    std::bitset<32> generateMachineCode(const Instruction& instruction);
    std::bitset<32> generateRTypeMachineCode(const std::string& opcode, const std::string& funct3, const std::string& funct7, const std::vector<std::string>& operands);
    std::bitset<32> generateITypeMachineCode(const std::string& opcode, const std::string& funct3, const std::vector<std::string>& operands);
    std::bitset<32> generateSTypeMachineCode(const std::string& opcode, const std::string& funct3, const std::vector<std::string>& operands);
    std::bitset<32> generateSBTypeMachineCode(const std::string& opcode, const std::string& funct3, const std::vector<std::string>& operands);
    std::bitset<32> generateUTypeMachineCode(const std::string& opcode, const std::vector<std::string>& operands);
    std::bitset<32> generateUJTypeMachineCode(const std::string& opcode, const std::vector<std::string>& operands);
};
