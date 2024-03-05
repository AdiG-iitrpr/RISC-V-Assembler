#include "Parser.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <filesystem>
#include <string>


using namespace std;

int main(int argc, char **argv) {

   if (argc != 2) {
      cout << "Usage: " << argv[0] << " <1:input file path>" << endl;
      return EXIT_FAILURE;
   }

   const char *inputPath = argv[1];

   cout << "===  assembler ===\n";
   cout << "Input file: " << inputPath << endl;

   return EXIT_SUCCESS;
}