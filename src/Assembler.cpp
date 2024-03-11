#include "Assembler.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <iomanip>

Assembler::Assembler(Lexer &lexer, Parser &parser, SymbolTable &symbolTable) : lexer(lexer), parser(parser), symbolTable(symbolTable), codeSegmentAddress(0x00000000), dataSegmentAddress(0x10000000) {
}

void Assembler::assemble(const std::string& inputFilePath, const std::string& outputFilePath) {

    std::string assemblyCode = readFile(inputFilePath);

    std::vector<Token> tokens = lexer.tokenize(assemblyCode, symbolTable);

    std::ofstream outputFile(outputFilePath);

    std::vector<std::pair<std::string, std::string>> instructionMachineCodes;

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
            std::string hexcode = binaryToHex(machineCode);
            instructionMachineCodes.push_back({decimalToHex(codeSegmentAddress, true, false), hexcode});
            codeSegmentAddress += 4;

        } else if (token.getType() == TokenType::DIRECTIVE) {
            handleDirective(token.getValue(), tokens, i, outputFile);
        }
    }

    for (const auto& [programCounter, machineCode] : instructionMachineCodes)
        outputFile <<  programCounter << " " << machineCode << std::endl;

    outputFile.close();
}

void Assembler::handleDirective(const std::string& directive, const std::vector<Token>& tokens, const size_t& tokenId, std::ofstream& outputFile) {

    std::string dataLabel = ".data";
    std::string textLabel = ".text";
    std::string charLabel = ".asciiz";
    std::string data;
    std::unordered_map<std::string, int>typeToSizeMap = {{".byte", 1}, {".half", 2}, {".word", 4}, {".dword", 8}};

    if (directive == dataLabel) {
        size_t j = tokenId + 1;
        while (tokens[j].getValue() != textLabel && j < tokens.size()) {

            if (tokens[j].getType() != TokenType::DIRECTIVE) {
                j += 1;
                continue;
            }
            std::string sizeType = tokens[j].getValue();
            int size = typeToSizeMap[sizeType];
            j += 1 ;
            while (tokens[j].getType() == TokenType::IMMEDIATE) {
                std::string immed = tokens[j].getValue();
                std::string imm = immedTypeToHexadecimal(immed, 2 * size);
                data += imm;
                j++;
            }
        }
        for (int i = 0; i < data.size(); i += 8) {

            std::string tempData = data.substr(i, 8);
            if (tempData.size() < 8) tempData = tempData + std::string(8 - tempData.size(), '0');
            std::string printData = "0x" + tempData.substr(6, 2) + tempData.substr(4, 2) + tempData.substr(2, 2) + tempData.substr(0, 2);

            outputFile << decimalToHex(dataSegmentAddress, false, true) << " " << printData << std::endl;
            dataSegmentAddress += 4;
        }
    }
}

std::string Assembler::immedTypeToHexadecimal(const std::string& imm, const int& size) {
    int immediate;
    std::string immed;
    if (imm[0] == '"') {
        for (int i = 1; i < imm.size() - 1; i++) {
            char c = imm[i];
            std::stringstream stream;
            stream << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(c);
            immed += stream.str();
        }
        immed += "00";
    } else {
        immediate = convertImmediateToInteger(imm);
        immed = decimalToHex(immediate, true, true).substr(2);
        if (immed.size() % 2 != 0) immed = "0" + immed;
        int currentLength = immed.length();
        int zerosNeeded = size - currentLength;
        if (zerosNeeded > 0) {
            immed = std::string(zerosNeeded, '0') + immed;
        }
        std::string tempImm = immed;
        immed = "";
        for (int i = 0; i < tempImm.size(); i += 2) {
            immed = tempImm.substr(i, 2) + immed;
        }
    }
    return immed;
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

std::string Assembler::decimalToHex(uint32_t decimal, bool trimzeros, bool upperCase) {
    std::stringstream stream;
    if (trimzeros) {
        if (upperCase)
            stream << std::hex << std::uppercase << decimal;
        else
            stream << std::hex << decimal;
    }
    else {
        if (upperCase)
            stream << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << decimal;
        else
            stream << std::hex << std::setw(8) << std::setfill('0') << decimal;
    }
    return "0x" + stream.str();
}

int Assembler::convertImmediateToInteger(const std::string& imm) {

    int immediate;
    if (imm[0] == '-') {

        if (imm.size() > 3 && imm[1] == '0' && (imm[2] == 'x' || imm[2] == 'X')) {
            immediate = -std::stoi(imm.substr(3), nullptr, 16);
        } else if (imm.size() > 3 && imm[1] == '0' && (imm[2] == 'b' || imm[2] == 'B')) {
            immediate = -std::stoi(imm.substr(3), nullptr, 2);
        } else {
            immediate = -std::stoi(imm.substr(1));
        }
    } else {
        if (imm.size() > 2 && imm[0] == '0' && (imm[1] == 'x' || imm[1] == 'X')) {
            immediate = std::stoi(imm.substr(2), nullptr, 16);
        } else if (imm.size() > 2 && imm[0] == '0' && (imm[1] == 'b' || imm[1] == 'B')) {
            immediate = std::stoi(imm.substr(2), nullptr, 2);
        } else {
            immediate = std::stoi(imm);
        }
    }

    return immediate;

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

    int rd = std::stoi(operands[0].substr(1));
    int rs1;
    std::string imm;

    // for load instructions immediate is second operand
    if (binaryStringToNumber(opcode) == 3) {
        rs1 = std::stoi(operands[2].substr(1));
        imm = operands[1];
    } else {
        imm = operands[2];
        rs1 = std::stoi(operands[1].substr(1));
    }

    int immediate = convertImmediateToInteger(imm);

    if (immediate < -2048 || immediate > 2047) {
        std::string allOperands;
        for (const auto& operand : operands) {
            allOperands += operand + " ";
        }
        throw std::runtime_error("Error: Immediate value out of range for I-type instruction (-2048 to 2047): " + allOperands);
    }

    machineCode = binaryStringToNumber(opcode)
                  | (rd << 7)
                  | (binaryStringToNumber(funct3) << 12)
                  | (rs1 << 15)
                  | (immediate << 20);

    return machineCode;
}

std::bitset<32> Assembler::generateSTypeMachineCode(const std::string& opcode, const std::string& funct3, const std::vector<std::string>& operands) {
    std::bitset<32> machineCode;

    int rs2 = std::stoi(operands[0].substr(1));
    int rs1 = std::stoi(operands[2].substr(1));
    std::string imm = operands[1];

    int immediate = convertImmediateToInteger(imm);

    if (immediate < -2048 || immediate > 2047) {
        std::string allOperands;
        for (const auto& operand : operands) {
            allOperands += operand + " ";
        }
        throw std::runtime_error("Error: Immediate value out of range for S-type instruction (-2048 to 2047): " + allOperands);
    }

    machineCode = binaryStringToNumber(opcode)
                  | ((immediate & 0x1F) << 7) // imm[0:4]
                  | (binaryStringToNumber(funct3) << 12)
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

    int immediate = convertImmediateToInteger(imm);

    if (immediate < -4096 || immediate > 4094) {
        std::string allOperands;
        for (const auto& operand : operands) {
            allOperands += operand + " ";
        }
        throw std::runtime_error("Error: Immediate value out of range for SB-type instruction (-4096 to 4094): " + allOperands);
    }

    machineCode = binaryStringToNumber(opcode)
                  | (((immediate & 0x800) >> 11) << 7) // imm[11]
                  | (((immediate & 0x1E) >> 1) << 8) // imm[1:4]
                  | (binaryStringToNumber(funct3) << 12)
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

    int immediate = convertImmediateToInteger(imm);

    if (immediate < -0 || immediate > 1048575) {
        std::string allOperands;
        for (const auto& operand : operands) {
            allOperands += operand + " ";
        }
        throw std::runtime_error("Error: Immediate value out of range for U-type instruction (0 to 1048575): " + allOperands);
    }

    machineCode = binaryStringToNumber(opcode)
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

    int immediate = convertImmediateToInteger(imm);

    if (immediate < -524288 || immediate > 524286) {
        std::string allOperands;
        for (const auto& operand : operands) {
            allOperands += operand + " ";
        }
        throw std::runtime_error("Error: Immediate value out of range for UJ-type instruction (-524288 to 524286): " + allOperands);
    }

    machineCode = binaryStringToNumber(opcode)
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
