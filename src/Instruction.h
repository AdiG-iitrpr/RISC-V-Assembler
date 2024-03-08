#pragma once

#include <string>
#include <vector>
#include <unordered_map>

enum class Type {
    R_TYPE,
    I_TYPE,
    S_TYPE,
    SB_TYPE,
    U_TYPE,
    UJ_TYPE,
    INVALID_TYPE
};

namespace InstructionInfo {

static const std::unordered_map<std::string, std::tuple<Type, std::string, std::string, std::string>> instructionMap = {
    // Format: {"instruction_name", {"type", opcode", "funct3", "funct7"}}

    {"add", {Type::R_TYPE, "0110011" , "000", "0000000"}},
    {"and", {Type::R_TYPE, "0110011", "111", "0000000"}},
    {"or", {Type::R_TYPE, "0110011" , "110", "0000000"}},
    {"sll", {Type::R_TYPE, "0110011", "001", "0000000"}},
    {"slt", {Type::R_TYPE, "0110011", "010", "0000000"}},
    {"sra", {Type::R_TYPE, "0110011", "101", "0100000"}},
    {"srl", {Type::R_TYPE, "0110011", "101", "0000000"}},
    {"sub", {Type::R_TYPE, "0110011", "000", "0100000"}},
    {"xor", {Type::R_TYPE, "0110011", "100", "0000000"}},
    {"addi", {Type::I_TYPE, "0010011", "000", ""}},
    {"andi", {Type::I_TYPE, "0010011", "111", ""}},
    {"ori", {Type::I_TYPE, "0010011", "110", ""}},
    {"lb", {Type::I_TYPE, "0000011", "000", ""}},
    {"ld", {Type::I_TYPE, "0000011", "011", ""}},
    {"lh", {Type::I_TYPE, "0000011", "001", ""}},
    {"lw", {Type::I_TYPE, "0000011", "010", ""}},
    {"jalr", {Type::I_TYPE, "1100111", "000", ""}},
    {"sb", {Type::S_TYPE, "0100011", "000", ""}},
    {"sw", {Type::S_TYPE, "0100011", "010", ""}},
    {"sd", {Type::S_TYPE, "0100011", "011", ""}},
    {"sh", {Type::S_TYPE, "0100011", "001", ""}},
    {"beq", {Type::SB_TYPE, "1100011", "000", ""}},
    {"bne", {Type::SB_TYPE, "1100011", "001", ""}},
    {"bge", {Type::SB_TYPE, "1100011", "101", ""}},
    {"blt", {Type::SB_TYPE, "1100011", "100", ""}},
    {"auipc", {Type::U_TYPE, "0010111", "", ""}},
    {"lui", {Type::U_TYPE, "0110111", "", ""}},
    {"jal", {Type::UJ_TYPE, "1101111", "", ""}}
};
}

class Instruction {
public:

    Instruction(Type type, const std::string& opcode, const std::string& funct3, const std::string& funct7, const std::vector<std::string>& operands);

    Type getType() const;
    const std::string& getOpcode() const;
    const std::string& getFunct3() const;
    const std::string& getFunct7() const;
    const std::vector<std::string>& getOperands() const;

private:
    Type type;
    std::string opcode;
    std::string funct3;
    std::string funct7;
    std::vector<std::string> operands;
};
