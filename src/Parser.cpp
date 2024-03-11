#include "Parser.h"

#include <string>
#include <vector>
#include <iostream>

Instruction Parser::parse(const std::vector<Token>& tokens, SymbolTable& symbolTable) {

    std::string instructionName = tokens[0].getValue();

    auto it = InstructionInfo::instructionMap.find(instructionName);

    if (it != InstructionInfo::instructionMap.end()) {
        InstructionType type = std::get<0>(it->second);
        std::string opcode = std::get<1>(it->second);
        std::string funct3 = std::get<2>(it->second);
        std::string funct7 = std::get<3>(it->second);
        std::vector<std::string> operands;

        checkValidInstruction(tokens, type, opcode);

        for (size_t i = 1; i < tokens.size(); ++i) {
            if (tokens[i].getType() == TokenType::LABEL) {

                int labelNextInstructionLineNumber = symbolTable.getLabelInstructionLineNumber(tokens[i].getValue());
                if (labelNextInstructionLineNumber == -1)
                    throw std::runtime_error("Label " + tokens[i].getValue() +  " used but not defined ");


                int immediate = 4 * labelNextInstructionLineNumber;
                if (type == InstructionType::R_TYPE or type == InstructionType::SB_TYPE or type == InstructionType::UJ_TYPE)
                    immediate -= 4 * tokens[i].getLineNumber();
                else
                    immediate -= 4;

                operands.push_back(std::to_string(immediate));
            } else
                operands.push_back(tokens[i].getValue());
        }

        return Instruction(type, opcode, funct3, funct7, operands);
    }

    return Instruction(InstructionType::INVALID_TYPE, "", "", "", {});

}

void Parser::checkValidInstruction(const std::vector<Token> &tokens, InstructionType type, std::string& opcode) {

    int numOperands = tokens.size() - 1;
    std::string cuurentNumOperands = std::to_string(numOperands);
    switch (type) {

    case InstructionType::R_TYPE:
        if (numOperands != 3)
            throw std::runtime_error("Expected 3 arguments but recieved " + cuurentNumOperands);

        for (size_t i = 1; i < tokens.size(); ++i) {
            if (tokens[i].getType() != TokenType::REGISTER)
                throw std::runtime_error(tokens[i].getValue() + " not a valid Register");
        }
        break;

    case InstructionType::I_TYPE:
        if (numOperands != 3)
            throw std::runtime_error("Expected 3 arguments but recieved " + cuurentNumOperands);

        if (tokens[1].getType() != TokenType::REGISTER)
            throw std::runtime_error(tokens[1].getValue() + " not a valid Register");

        if (opcode == "0000011") {

            if (tokens[3].getType() != TokenType::REGISTER)
                throw std::runtime_error(tokens[3].getValue() + " not a valid Register");
        } else {

            if (tokens[2].getType() != TokenType::REGISTER)
                throw std::runtime_error(tokens[2].getValue() + " not a valid Register");
        }
        break;

    case InstructionType::S_TYPE:
        if (numOperands != 3)
            throw std::runtime_error("Expected 3 arguments but recieved " + cuurentNumOperands);

        for (size_t i = 1; i < tokens.size(); i += 2) {
            if (tokens[i].getType() != TokenType::REGISTER)
                throw std::runtime_error(tokens[i].getValue() + " not a valid Register");
        }
        break;

    case InstructionType::SB_TYPE:
        if (numOperands != 3)
            throw std::runtime_error("Expected 3 arguments but recieved " + cuurentNumOperands);

        for (size_t i = 1; i < tokens.size() - 1 ; ++i) {
            if (tokens[i].getType() != TokenType::REGISTER)
                throw std::runtime_error(tokens[i].getValue() + " not a valid Register");
        }
        break;

    case InstructionType::U_TYPE:
        if (numOperands != 2)
            throw std::runtime_error("Expected 2 arguments but recieved " + cuurentNumOperands);

        if (tokens[1].getType() != TokenType::REGISTER)
            throw std::runtime_error(tokens[1].getValue() + " not a valid Register");
        break;

    case InstructionType::UJ_TYPE:
        if (numOperands != 2 and numOperands != 1)
            throw std::runtime_error("Expected 2 or 1 arguments but recieved " + cuurentNumOperands);

        if (numOperands == 2 and tokens[1].getType() != TokenType::REGISTER)
            throw std::runtime_error(tokens[1].getValue() + " not a valid Register");
        break;

    default:
        break;


    }

}