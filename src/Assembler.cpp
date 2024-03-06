#include "Assembler.h"
#include "Token.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

Assembler::Assembler(Lexer &lexer, Parser &parser, SymbolTable &symbolTable) : codeSegmentAddress(0x00000000), dataSegmentAddress(0x10000000), lexer(lexer), parser(parser), symbolTable(symbolTable) {
}

void Assembler::assemble(const std::string& inputFilePath, const std::string& outputFilePath) {

    std::string assemblyCode = readFile(inputFilePath);

    std::vector<Token> tokens = lexer.tokenize(assemblyCode);

    std::ofstream outputFile(outputFilePath);

    for (size_t i = 0; i < tokens.size(); ++i) {
        Token token = tokens[i];

        if (token.getType() == TokenType::INSTRUCTION) {
            std::vector<Token> instructionTokens;
            while (i < tokens.size() && tokens[i].getType() == TokenType::INSTRUCTION) {
                instructionTokens.push_back(tokens[i]);
                ++i;
            }
            --i;
            std::vector<Instruction> parsedInstructions = parser.parse(instructionTokens, symbolTable);
        } else if (token.getType() == TokenType::DIRECTIVE) {
            handleDirective(token.getValue(), tokens);
        }
    }

    outputFile.close();
}

void Assembler::handleDirective(const std::string& directive, const std::vector<Token>& tokens) {

}


std::string Assembler::readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filePath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

Assembler::~Assembler() {
    // empty
}
