#include "Instruction.h"

Instruction::Instruction(InstructionType type, const std::string& opcode, const std::string& funct3, const std::string& funct7, const std::vector<std::string>& operands)
    : type(type), opcode(opcode), funct3(funct3), funct7(funct7), operands(operands) {}


InstructionType Instruction::getType() const {
    return type;
}

const std::string& Instruction::getOpcode() const {
    return opcode;
}

const std::string& Instruction::getFunct3() const {
    return funct3;
}

const std::string& Instruction::getFunct7() const {
    return funct7;
}

const std::vector<std::string>& Instruction::getOperands() const {
    return operands;
}