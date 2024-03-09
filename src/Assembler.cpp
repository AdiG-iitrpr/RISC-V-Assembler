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
            outputFile << decimalToHex(codeSegmentAddress) << " " << hexcode << std::endl;
            codeSegmentAddress += 4;

        } else if (token.getType() == TokenType::DIRECTIVE) {
            handleDirective(token.getValue(), tokens, i);
        }
    }

    outputFile.close();
}

void Assembler::handleDirective(const std::string& directive, const std::vector<Token>& tokens, const size_t& tokenId) {
    std::string dataLabel = ".data";
    std::string textLabel = ".text";
    std::string charLabel = ".asciiz";
    std::string dataAddress = "0x10000000";
    std::unordered_map<std::string, int>typeToSizeMap = {{".byte",1},{".half",2},{".word",4},{".dword",8}};
    if(directive == dataLabel){
        std::cout<<"--Data Segment--"<<std::endl;
        size_t j = tokenId+1;
        while(tokens[j].getValue() != textLabel){
            if(tokens[j].getType() != TokenType::DIRECTIVE){
                j+=1;
                continue;
            }
            std::string sizeType = tokens[j].getValue();
            int size = typeToSizeMap[sizeType];
            j +=1 ;
            while(tokens[j].getType() == TokenType::IMMEDIATE){
                std::string immed = tokens[j].getValue();
                std::string imm = immedTypeToHexadecimal(immed);
                std::cout<<dataAddress<<" "<<imm<<std::endl;
                uint32_t address = std::stoul(dataAddress, nullptr, 16);
                if(sizeType == charLabel){
                    size = immed.size()-2;
                    address += size;
                }
                else{
                    address += size;
                }
                std::stringstream stream;
                stream << "0x" << std::uppercase << std::setw(8) << std::setfill('0') << std::hex << address;
                dataAddress = stream.str();
                j++;
            }
        }
    }
}

std::string Assembler::immedTypeToHexadecimal(const std::string& imm){
    int immediate;
    std::string immed = "0x";
    if (imm[0] == '"'){
        for (int i = 1; i < imm.size()-1; i++) {
            char c = imm[i];
            std::stringstream stream;
            stream << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(c);
            immed += stream.str();
        }
    } else if (imm[0] == '-') {
        if (imm.size() > 3 && imm[1] == '0' && (imm[2] == 'x' || imm[2] == 'X')) {
            immediate = -std::stoi(imm.substr(3), nullptr, 16);
        } else if (imm.size() > 3 && imm[1] == '0' && (imm[2] == 'b' || imm[2] == 'B')) {
            immediate = -std::stoi(imm.substr(3), nullptr, 2);
        } else {
            immediate = -std::stoi(imm.substr(1));
        }
        std::stringstream stream;
        stream << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << immediate;
        immed += stream.str();
    } else {
        if (imm.size() > 2 && imm[0] == '0' && (imm[1] == 'x' || imm[1] == 'X')) {
            immediate = std::stoi(imm.substr(2), nullptr, 16);
        } else if (imm.size() > 2 && imm[0] == '0' && (imm[1] == 'b' || imm[1] == 'B')) {
            immediate = std::stoi(imm.substr(2), nullptr, 2);
        } else {
            immediate = std::stoi(imm);
        }
        std::stringstream stream;
        stream << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << immediate;
        immed += stream.str();
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

std::string Assembler::decimalToHex(uint32_t decimal) {
    std::stringstream stream;
    stream << std::hex << decimal;
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

    machineCode = binaryStringToNumber(opcode)
                  | (rd << 7)
                  | ((immediate & 0xFFFFF000) << 12); // imm[12:31]
    return machineCode;
}

std::bitset<32> Assembler::generateUJTypeMachineCode(const std::string& opcode, const std::vector<std::string>& operands) {
    std::bitset<32> machineCode;

    int rd = std::stoi(operands[0].substr(1));
    std::string imm = operands[1];

    int immediate = convertImmediateToInteger(imm);

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
