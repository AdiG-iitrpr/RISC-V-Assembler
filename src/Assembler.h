#pragma once

#include "Lexer.h"
#include "Parser.h"
#include "SymbolTable.h"
#include "Token.h"

#include <string>
#include <vector>

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
};
