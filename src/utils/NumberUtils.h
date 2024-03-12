#pragma once

#include <bitset>
#include <string>

class NumberUtils {
public:
    static std::string decimalToHex(uint32_t decimal, bool trimzeros, bool upperCase);
    static std::string binaryToHex(const std::bitset<32>& bits);
    static int binaryStringToNumber(const std::string& binaryString);
    static int convertStringToInteger(const std::string& imm);
    static bool isHexadecimal(const std::string& str);
    static bool isBinary(const std::string& str);
};
