#pragma once

#include "../../utils/Enums.h"
#include <string>
#include <vector>
#include <unordered_map>


namespace InstructionInfo {

static const std::unordered_map<std::string, std::tuple<InstructionType, std::string, std::string, std::string>> instructionMap = {
    // Format: {"instruction_name", {"type", opcode", "funct3", "funct7"}}

    {"add", {InstructionType::R_TYPE, "0110011" , "000", "0000000"}},
    {"and", {InstructionType::R_TYPE, "0110011", "111", "0000000"}},
    {"or", {InstructionType::R_TYPE, "0110011" , "110", "0000000"}},
    {"sll", {InstructionType::R_TYPE, "0110011", "001", "0000000"}},
    {"slt", {InstructionType::R_TYPE, "0110011", "010", "0000000"}},
    {"sra", {InstructionType::R_TYPE, "0110011", "101", "0100000"}},
    {"srl", {InstructionType::R_TYPE, "0110011", "101", "0000000"}},
    {"sub", {InstructionType::R_TYPE, "0110011", "000", "0100000"}},
    {"xor", {InstructionType::R_TYPE, "0110011", "100", "0000000"}},
    {"mul", {InstructionType::R_TYPE, "0110011", "000", "0000001"}},
    {"div", {InstructionType::R_TYPE, "0110011", "100", "0000001"}},
    {"rem", {InstructionType::R_TYPE, "0110011", "110", "0000001"}},
    {"addi", {InstructionType::I_TYPE, "0010011", "000", ""}},
    {"andi", {InstructionType::I_TYPE, "0010011", "111", ""}},
    {"ori", {InstructionType::I_TYPE, "0010011", "110", ""}},
    {"xori", {InstructionType::I_TYPE, "0010011", "100", ""}},
    {"lb", {InstructionType::I_TYPE, "0000011", "000", ""}},
    {"ld", {InstructionType::I_TYPE, "0000011", "011", ""}},
    {"lh", {InstructionType::I_TYPE, "0000011", "001", ""}},
    {"lw", {InstructionType::I_TYPE, "0000011", "010", ""}},
    {"slli", {InstructionType::I_TYPE, "0010011", "001", "0000000"}},
    {"slti", {InstructionType::I_TYPE, "0010011", "010", "0000000"}},
    {"srai", {InstructionType::I_TYPE, "0010011", "101", "0100000"}},
    {"srli", {InstructionType::I_TYPE, "0010011", "101", "0000000"}},
    {"jalr", {InstructionType::I_TYPE, "1100111", "000", ""}},
    {"sb", {InstructionType::S_TYPE, "0100011", "000", ""}},
    {"sw", {InstructionType::S_TYPE, "0100011", "010", ""}},
    {"sd", {InstructionType::S_TYPE, "0100011", "011", ""}},
    {"sh", {InstructionType::S_TYPE, "0100011", "001", ""}},
    {"beq", {InstructionType::SB_TYPE, "1100011", "000", ""}},
    {"bne", {InstructionType::SB_TYPE, "1100011", "001", ""}},
    {"bge", {InstructionType::SB_TYPE, "1100011", "101", ""}},
    {"blt", {InstructionType::SB_TYPE, "1100011", "100", ""}},
    {"auipc", {InstructionType::U_TYPE, "0010111", "", ""}},
    {"lui", {InstructionType::U_TYPE, "0110111", "", ""}},
    {"jal", {InstructionType::UJ_TYPE, "1101111", "", ""}}
};
}

class Instruction {
public:

    Instruction(InstructionType type, const std::string& opcode, const std::string& funct3, const std::string& funct7, const std::vector<std::string>& operands);

    InstructionType getType() const;
    const std::string& getOpcode() const;
    const std::string& getFunct3() const;
    const std::string& getFunct7() const;
    const std::vector<std::string>& getOperands() const;

private:
    InstructionType type;
    std::string opcode;
    std::string funct3;
    std::string funct7;
    std::vector<std::string> operands;
};
