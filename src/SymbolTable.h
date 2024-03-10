#pragma once

#include <unordered_map>
#include <string>

class SymbolTable {
public:

    void addLabel(const std::string& label, int lineNumber);
    int getLabelInstructionLineNumber(const std::string& label) const;

private:
    std::unordered_map<std::string, int> labels;
};
