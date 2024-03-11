#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class FileUtils {
public:
    static std::string readFile(const std::string& filePath);
};

