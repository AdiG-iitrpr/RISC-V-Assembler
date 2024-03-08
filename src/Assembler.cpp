#include "Assembler.h"
#include "Instruction.h"
#include "Token.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <bitset>
#include <iomanip>

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
            std::bitset<32> machineCode = generateMachineCode(parsedInstruction);
            std::string hexcode = binaryToHex(machineCode);
            std::cout << hexcode << std::endl;

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

std::bitset<32> Assembler::generateMachineCode(const Instruction& instruction) {

    std::bitset<32> machineCode;
    Type type = instruction.getType();
    std::string opcode = instruction.getOpcode();
    std::string funct3 = instruction.getFunct3();
    std::string funct7 = instruction.getFunct7();
    std::vector<std::string> operands = instruction.getOperands();

    switch (type) {
    case Type::R_TYPE:
        machineCode = generateRTypeMachineCode(opcode, funct3, funct7, operands);
        break;
    case Type::I_TYPE:
        machineCode = generateITypeMachineCode(opcode, funct3, operands);
        break;
    case Type::S_TYPE:
        machineCode = generateSTypeMachineCode(opcode, funct3, operands);
        break;
    case Type::SB_TYPE:
        machineCode = generateSBTypeMachineCode(opcode, funct3, operands);
        break;
    case Type::U_TYPE:
        machineCode = generateUTypeMachineCode(opcode, operands);
        break;
    case Type::UJ_TYPE:
        machineCode = generateUJTypeMachineCode(opcode, operands);
        break;
    default:
        break;
    }

    return machineCode;
}

int Assembler::binaryStringToNumber(const std::string& binaryString) {
    return std::bitset<32>(binaryString).to_ulong();
}

std::string Assembler::binaryToHex(const std::bitset<32>&bits) {
    unsigned long long intVal = bits.to_ullong();
    std::stringstream stream;
    stream << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << intVal;
    return "0x" + stream.str();
}

std::bitset<32> Assembler::generateRTypeMachineCode(const std::string& opcode, const std::string& funct3, const std::string& funct7, const std::vector<std::string>& operands) {

    std::bitset<32> machineCode;

    int rd = std::stoi(operands[0].substr(1));
    int rs1 = std::stoi(operands[1].substr(1));
    int rs2 = std::stoi(operands[2].substr(1));

    machineCode = binaryStringToNumber(opcode)
                  | (rd << 7)
                  | (binaryStringToNumber(funct3) << 12)
                  | (rs1 << 15)
                  | (rs2 << 20)
                  | (binaryStringToNumber(funct7) << 25);

    return machineCode;
}

std::bitset<32> Assembler::generateITypeMachineCode(const std::string& opcode, const std::string& funct3, const std::vector<std::string>& operands) {
    std::bitset<32> machineCode;
    return machineCode;
}

std::bitset<32> Assembler::generateSTypeMachineCode(const std::string& opcode, const std::string& funct3, const std::vector<std::string>& operands) {
    std::bitset<32> machineCode;
    return machineCode;
}

std::bitset<32> Assembler::generateSBTypeMachineCode(const std::string& opcode, const std::string& funct3, const std::vector<std::string>& operands) {
    std::bitset<32> machineCode;
    return machineCode;
}

std::bitset<32> Assembler::generateUTypeMachineCode(const std::string& opcode, const std::vector<std::string>& operands) {
    std::bitset<32> machineCode;
    return machineCode;
}

std::bitset<32> Assembler::generateUJTypeMachineCode(const std::string& opcode, const std::vector<std::string>& operands) {
    std::bitset<32> machineCode;
    return machineCode;
}


Assembler::~Assembler() {
    // empty
}
