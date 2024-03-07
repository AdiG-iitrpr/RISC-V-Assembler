#include "Assembler.h"
#include "Instruction.h"
#include "Token.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

Assembler::Assembler(Lexer &lexer, Parser &parser, SymbolTable &symbolTable) : codeSegmentAddress(0x00000000), dataSegmentAddress(0x10000000), lexer(lexer), parser(parser), symbolTable(symbolTable) {
}

void Assembler::assemble(const std::string& inputFilePath, const std::string& outputFilePath) {

    std::string assemblyCode = readFile(inputFilePath);

    std::vector<Token> tokens = lexer.tokenize(assemblyCode, symbolTable);

    std::ofstream outputFile(outputFilePath);

    for (const auto& pair : symbolTable.getLabels()) {
        std::cout << "Label: " << pair.first << ", Line Number: " << pair.second << std::endl;
    }

    for (size_t i = 0; i < tokens.size(); ++i) {
        Token token = tokens[i];

        if (token.getType() == TokenType::INSTRUCTION) {
            std::vector<Token> instructionTokens;
            while (i < tokens.size() && tokens[i].getLineNumber() == token.getLineNumber()) {
                instructionTokens.push_back(tokens[i]);
                ++i;
            }
            --i;

            for (int j = 0; j < instructionTokens.size(); j++)
                std::cout << instructionTokens[j].getValue() << " ";
            std::cout << std::endl;

            Instruction parsedInstruction = parser.parse(instructionTokens, symbolTable);
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
    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty()) {
            buffer << line << '\n';
        }
    }

    return buffer.str();
}


Assembler::~Assembler() {
    // empty
}
