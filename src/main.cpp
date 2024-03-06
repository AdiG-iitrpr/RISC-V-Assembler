#include "Assembler.h"
#include "Lexer.h"
#include "Parser.h"
#include "SymbolTable.h"

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

   string inputPath = argv[1];

   cout << "===  assembler ===\n";
   cout << "Input file: " << inputPath << endl;

   string outputPath = "../output/example1.mc";

   unique_ptr<Lexer> lexer(new Lexer());
   unique_ptr<Parser> parser(new Parser());
   unique_ptr<SymbolTable> symbolTable(new SymbolTable());

   unique_ptr<Assembler> assembler(new Assembler(*lexer, *parser, *symbolTable));
   assembler->assemble(inputPath, outputPath);

   return EXIT_SUCCESS;
}