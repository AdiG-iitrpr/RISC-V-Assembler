#include "Parser.h"
#include "Token.h"
#include <string>
#include <vector>
#include <iostream>

Instruction Parser::parse(const std::vector<Token>& tokens, SymbolTable& symbolTable) {

    std::string instructionName = tokens[0].getValue();

    auto it = InstructionInfo::instructionMap.find(instructionName);

    if (it != InstructionInfo::instructionMap.end()) {
        Type type = std::get<0>(it->second);
        std::string opcode = std::get<1>(it->second);
        std::string funct3 = std::get<2>(it->second);
        std::string funct7 = std::get<3>(it->second);
        std::vector<std::string> operands;

        for (size_t i = 1; i < tokens.size(); ++i) {
            if (tokens[i].getType() == TokenType::LABEL) {
                int immediate = 4 * (symbolTable.getLabelInstructionLineNumber(tokens[i].getValue()) - tokens[i].getLineNumber());
                operands.push_back(std::to_string(immediate));
                std::cout << "immediate is " << immediate << std::endl;
            } else
                operands.push_back(tokens[i].getValue());
        }

        return Instruction(type, opcode, funct3, funct7, operands);
    }

    return Instruction(Type::INVALID_TYPE, "", "", "", {});

}