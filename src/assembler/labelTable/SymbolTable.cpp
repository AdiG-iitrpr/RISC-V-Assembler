#include "SymbolTable.h"

void SymbolTable::addLabel(const std::string& label, int lineNumber) {
    labels[label] = lineNumber;
}

int SymbolTable::getLabelInstructionLineNumber(const std::string& label) const {
    auto it = labels.find(label);
    if (it != labels.end()) {
        return it->second;
    }

    return -1;
}