#include "Assembler.h"
#include "../utils/FileUtils.h"
#include "../utils/NumberUtils.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <iomanip>

Assembler::Assembler(Lexer &lexer, Parser &parser, SymbolTable &symbolTable) : lexer(lexer), parser(parser), symbolTable(symbolTable), codeSegmentAddress(0x00000000), dataSegmentAddress(0x10000000) {
}

void Assembler::assemble(const std::string& inputFilePath, const std::string& outputFilePath) {

    std::string assemblyCode = FileUtils::readFile(inputFilePath);

    std::vector<Token> tokens = lexer.tokenize(assemblyCode, symbolTable);

    std::ofstream outputFile(outputFilePath);

    std::vector<std::pair<std::string, std::string>> instructionMachineCodes;

    std::string dataSegmentBuffer;

    for (size_t i = 0; i < tokens.size(); ++i) {
        Token token = tokens[i];

        if (token.getType() == TokenType::INSTRUCTION) {
            std::vector<Token> instructionTokens;
            while (i < tokens.size() && tokens[i].getLineNumber() == token.getLineNumber()) {
                instructionTokens.push_back(tokens[i]);
                ++i;
            }
            --i;

            Instruction parsedInstruction = parser.parse(instructionTokens, symbolTable);
            std::bitset<32> machineCode = generateMachineCode(parsedInstruction);
            std::string hexcode = NumberUtils::binaryToHex(machineCode);

            outputFile << NumberUtils::decimalToHex(codeSegmentAddress, true, false) << " " << hexcode << std::endl;
            codeSegmentAddress += 4;

        } else if (token.getType() == TokenType::DIRECTIVE) {
            handleDirective(token.getValue(), tokens, i, dataSegmentBuffer);
        }
    }

    outputFile << " " << std::endl;

    for (size_t i = 0; i < dataSegmentBuffer.size(); i += 8) {

        std::string dataSegmentValue = dataSegmentBuffer.substr(i, 8);
        if (dataSegmentValue.size() < 8) dataSegmentValue = dataSegmentValue + std::string(8 - dataSegmentValue.size(), '0');
        std::string addressValue = "0x" + dataSegmentValue.substr(6, 2) + dataSegmentValue.substr(4, 2) + dataSegmentValue.substr(2, 2) + dataSegmentValue.substr(0, 2);
        outputFile << NumberUtils::decimalToHex(dataSegmentAddress, false, true) << " " << addressValue << std::endl;
        dataSegmentAddress += 4;

    }

    outputFile.close();
}

void Assembler::handleDirective(const std::string& directive, const std::vector<Token>& tokens, const size_t& tokenId, std::string& dataSegmentBuffer) {

    DirectiveType directiveType = lexer.getDirectiveType(directive);

    std::unordered_map<DirectiveType, int>typeToSizeMap = {{DirectiveType::BYTE, 1}, {DirectiveType::HALF, 2}, {DirectiveType::WORD, 4}, {DirectiveType::DWORD, 8}};

    if (directiveType == DirectiveType::DATA) {
        size_t j = tokenId + 1;
        while (lexer.getDirectiveType(tokens[j].getValue()) != DirectiveType::TEXT && j < tokens.size()) {

            if (tokens[j].getType() != TokenType::DIRECTIVE) {
                j += 1;
                continue;
            }

            DirectiveType sizeType = lexer.getDirectiveType(tokens[j].getValue());
            int size = typeToSizeMap[sizeType];
            j += 1 ;
            while (tokens[j].getType() == TokenType::IMMEDIATE) {
                std::string immediate = tokens[j].getValue();
                std::string immediateHexadecimal = convertImmediateToHexadecimalFormat(immediate, 2 * size);
                dataSegmentBuffer += immediateHexadecimal;
                j++;

            }
        }
    }
}

std::string Assembler::convertImmediateToHexadecimalFormat(const std::string& immediate, const int& size) {

    std::string immediateHexadecimal;
    if (immediate[0] == '"') {
        for (size_t i = 1; i < immediate.size() - 1; i++) {
            char c = immediate[i];
            std::stringstream stream;
            stream << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(c);
            immediateHexadecimal += stream.str();
        }
        immediateHexadecimal += "00";
    } else {

        immediateHexadecimal = NumberUtils::decimalToHex(NumberUtils::convertStringToInteger(immediate), true, true).substr(2);
        if (immediateHexadecimal.size() % 2 != 0) immediateHexadecimal = "0" + immediateHexadecimal;

        int currentLength = immediateHexadecimal.length();
        int zerosNeeded = size - currentLength;

        if (zerosNeeded > 0)
            immediateHexadecimal = std::string(zerosNeeded, '0') + immediateHexadecimal;

        std::string formattedHex;
        for (size_t i = 0; i < immediateHexadecimal.size(); i += 2)
            formattedHex = immediateHexadecimal.substr(i, 2) + formattedHex;

        immediateHexadecimal = formattedHex;
    }
    return immediateHexadecimal;
}

std::bitset<32> Assembler::generateMachineCode(const Instruction& instruction) {

    std::bitset<32> machineCode;
    InstructionType type = instruction.getType();
    std::string opcode = instruction.getOpcode();
    std::string funct3 = instruction.getFunct3();
    std::string funct7 = instruction.getFunct7();
    std::vector<std::string> operands = instruction.getOperands();

    switch (type) {
    case InstructionType::R_TYPE:
        machineCode = generateRTypeMachineCode(opcode, funct3, funct7, operands);
        break;
    case InstructionType::I_TYPE:
        machineCode = generateITypeMachineCode(opcode, funct3, operands);
        break;
    case InstructionType::S_TYPE:
        machineCode = generateSTypeMachineCode(opcode, funct3, operands);
        break;
    case InstructionType::SB_TYPE:
        machineCode = generateSBTypeMachineCode(opcode, funct3, operands);
        break;
    case InstructionType::U_TYPE:
        machineCode = generateUTypeMachineCode(opcode, operands);
        break;
    case InstructionType::UJ_TYPE:
        machineCode = generateUJTypeMachineCode(opcode, operands);
        break;
    default:
        break;
    }

    return machineCode;
}

std::bitset<32> Assembler::generateRTypeMachineCode(const std::string& opcode, const std::string& funct3, const std::string& funct7, const std::vector<std::string>& operands) {

    std::bitset<32> machineCode;

    int rd = std::stoi(operands[0].substr(1));
    int rs1 = std::stoi(operands[1].substr(1));
    int rs2 = std::stoi(operands[2].substr(1));

    machineCode = NumberUtils::binaryStringToNumber(opcode)
                  | (rd << 7)
                  | (NumberUtils::binaryStringToNumber(funct3) << 12)
                  | (rs1 << 15)
                  | (rs2 << 20)
                  | (NumberUtils::binaryStringToNumber(funct7) << 25);

    return machineCode;
}

std::bitset<32> Assembler::generateITypeMachineCode(const std::string& opcode, const std::string& funct3, const std::vector<std::string>& operands) {
    std::bitset<32> machineCode;

    int rd = std::stoi(operands[0].substr(1));
    int rs1;
    std::string imm;

    // for load instructions immediate is second operand
    if (NumberUtils::binaryStringToNumber(opcode) == 3) {
        rs1 = std::stoi(operands[2].substr(1));
        imm = operands[1];
    } else {
        imm = operands[2];
        rs1 = std::stoi(operands[1].substr(1));
    }

    int immediate = NumberUtils::convertStringToInteger(imm);

    if (immediate < -2048 || immediate > 2047) {
        std::string allOperands;
        for (const auto& operand : operands) {
            allOperands += operand + " ";
        }
        throw std::runtime_error("Error: Immediate value out of range for I-type instruction (-2048 to 2047): " + allOperands);
    }

    machineCode = NumberUtils::binaryStringToNumber(opcode)
                  | (rd << 7)
                  | (NumberUtils::binaryStringToNumber(funct3) << 12)
                  | (rs1 << 15)
                  | (immediate << 20);

    return machineCode;
}

std::bitset<32> Assembler::generateSTypeMachineCode(const std::string& opcode, const std::string& funct3, const std::vector<std::string>& operands) {
    std::bitset<32> machineCode;

    int rs2 = std::stoi(operands[0].substr(1));
    int rs1 = std::stoi(operands[2].substr(1));
    std::string imm = operands[1];

    int immediate = NumberUtils::convertStringToInteger(imm);

    if (immediate < -2048 || immediate > 2047) {
        std::string allOperands;
        for (const auto& operand : operands) {
            allOperands += operand + " ";
        }
        throw std::runtime_error("Error: Immediate value out of range for S-type instruction (-2048 to 2047): " + allOperands);
    }

    machineCode = NumberUtils::binaryStringToNumber(opcode)
                  | ((immediate & 0x1F) << 7) // imm[0:4]
                  | (NumberUtils::binaryStringToNumber(funct3) << 12)
                  | (rs1 << 15)
                  | (rs2 << 20)
                  | (((immediate & 0xFE0) >> 5) << 25);  // (imm[5:11])

    return machineCode;
}

std::bitset<32> Assembler::generateSBTypeMachineCode(const std::string& opcode, const std::string& funct3, const std::vector<std::string>& operands) {
    std::bitset<32> machineCode;

    int rs1 = std::stoi(operands[0].substr(1));
    int rs2 = std::stoi(operands[1].substr(1));
    std::string imm = operands[2];

    int immediate = NumberUtils::convertStringToInteger(imm);

    if (immediate < -4096 || immediate > 4094) {
        std::string allOperands;
        for (const auto& operand : operands) {
            allOperands += operand + " ";
        }
        throw std::runtime_error("Error: Immediate value out of range for SB-type instruction (-4096 to 4094): " + allOperands);
    }

    machineCode = NumberUtils::binaryStringToNumber(opcode)
                  | (((immediate & 0x800) >> 11) << 7) // imm[11]
                  | (((immediate & 0x1E) >> 1) << 8) // imm[1:4]
                  | (NumberUtils::binaryStringToNumber(funct3) << 12)
                  | (rs1 << 15)
                  | (rs2 << 20)
                  | (((immediate & 0x7E0) >> 5) << 25) // (imm[5:10])
                  | (((immediate & 0x1000) >> 12) << 31); // imm[12]
    return machineCode;
}

std::bitset<32> Assembler::generateUTypeMachineCode(const std::string& opcode, const std::vector<std::string>& operands) {
    std::bitset<32> machineCode;

    int rd = std::stoi(operands[0].substr(1));
    std::string imm = operands[1];

    int immediate = NumberUtils::convertStringToInteger(imm);

    if (immediate < -0 || immediate > 1048575) {
        std::string allOperands;
        for (const auto& operand : operands) {
            allOperands += operand + " ";
        }
        throw std::runtime_error("Error: Immediate value out of range for U-type instruction (0 to 1048575): " + allOperands);
    }

    machineCode = NumberUtils::binaryStringToNumber(opcode)
                  | (rd << 7)
                  | ((immediate & 0x000FFFFF) << 12); // imm left shifted by 12 and then imm[12:31] implies imm[0:19]
    return machineCode;
}

std::bitset<32> Assembler::generateUJTypeMachineCode(const std::string& opcode, const std::vector<std::string>& operands) {
    std::bitset<32> machineCode;

    int rd;
    std::string imm;

    if (operands.size() == 1) {
        rd = 1;
        imm = operands[0];
    } else {
        rd = std::stoi(operands[0].substr(1));
        imm = operands[1];
    }

    int immediate = NumberUtils::convertStringToInteger(imm);

    if (immediate < -524288 || immediate > 524286) {
        std::string allOperands;
        for (const auto& operand : operands) {
            allOperands += operand + " ";
        }
        throw std::runtime_error("Error: Immediate value out of range for UJ-type instruction (-524288 to 524286): " + allOperands);
    }

    machineCode = NumberUtils::binaryStringToNumber(opcode)
                  | (rd << 7)
                  | (((immediate & 0xFF000) >> 12) << 12) // imm[12:19]
                  | (((immediate & 0x800) >> 11) << 20) // imm[10]
                  | (((immediate & 0x7FE) >> 1) << 21) // imm[1:10]
                  | (((immediate & 0x100000) >> 20) << 31); // imm[20]

    return machineCode;
}


Assembler::~Assembler() {
    // empty
}
