#include "NumberUtils.h"

#include <sstream>
#include <iomanip>

int NumberUtils::binaryStringToNumber(const std::string& binaryString) {
	return std::bitset<32>(binaryString).to_ulong();
}

std::string NumberUtils::binaryToHex(const std::bitset<32>&bits) {
	unsigned long long intVal = bits.to_ullong();
	std::stringstream stream;
	stream << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << intVal;
	return "0x" + stream.str();
}

std::string NumberUtils::decimalToHex(uint32_t decimal, bool trimzeros, bool upperCase) {
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

int NumberUtils::convertStringToInteger(const std::string& num) {

	int number;
	if (num[0] == '-') {

		if (num.size() > 3 && num[1] == '0' && num[2] == 'x' ) {
			number = -std::stoi(num.substr(3), nullptr, 16);
		} else if (num.size() > 3 && num[1] == '0' && num[2] == 'b') {
			number = -std::stoi(num.substr(3), nullptr, 2);
		} else {
			number = -std::stoi(num.substr(1));
		}
	} else {
		if (num.size() > 2 && num[0] == '0' && num[1] == 'x') {
			number = std::stoi(num.substr(2), nullptr, 16);
		} else if (num.size() > 2 && num[0] == '0' && num[1] == 'b') {
			number = std::stoi(num.substr(2), nullptr, 2);
		} else {
			number = std::stoi(num);
		}
	}

	return number;

}

bool NumberUtils::isHexadecimal(const std::string& str) {
	for (char c : str) {
		if (!std::isxdigit(c)) {
			return false;
		}
	}
	return true;
}

bool NumberUtils::isBinary(const std::string& str) {
	for (char c : str) {
		if (c != '0' && c != '1') {
			return false;
		}
	}
	return true;
}