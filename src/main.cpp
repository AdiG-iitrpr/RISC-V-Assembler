#include "Assembler.h"
#include "Lexer.h"
#include "Parser.h"
#include "SymbolTable.h"

#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <memory>
#include <string>

std::string generateOutputPath(const std::string& inputPath) {

   std::filesystem::path inputFilename = std::filesystem::path(inputPath).filename();
   std::string filenameWithoutExtension = inputFilename.stem().string();

   std::filesystem::path outputPath = "../output/" + filenameWithoutExtension + ".mc";

   return outputPath.string();
}

int main(int argc, char **argv) {

   if (argc != 2) {
      std::cout << "Usage: " << argv[0] << " <1:input file path>" << std::endl;
      return EXIT_FAILURE;
   }

   std::string inputPath = argv[1];

   std::cout << "===  assembler ===\n";
   std::cout << "Input file: " << inputPath << std::endl;

   std::string outputPath = generateOutputPath(inputPath);

   std::unique_ptr<Lexer> lexer(new Lexer());
   std::unique_ptr<Parser> parser(new Parser());
   std::unique_ptr<SymbolTable> symbolTable(new SymbolTable());

   std::unique_ptr<Assembler> assembler(new Assembler(*lexer, *parser, *symbolTable));
   assembler->assemble(inputPath, outputPath);

   return EXIT_SUCCESS;
}