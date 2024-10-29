#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "lexer/include/lexer.hpp"

#define VERSION "0.0.3"

// Help information [-h, --help]
void printHelp() {
  std::cout << "MathSol v" << VERSION << " mathsol language interpreter\n";
  std::cout << "(C) 2024-now madc0der\n\n";
  std::cout << "Usage:\n  mathsol [option] [file]\n\n";
  std::cout << "Options:\n";
  std::cout << "  -c, --command  : program passed in as a string\n";
  std::cout << "  -h, --help     : display this help information\n";
  std::cout << "  -V, --version  : display version information\n\n";
  std::cout << "Examples:\n";
  std::cout << "  mathsol                : Runs the interpreter interactively\n";
  std::cout << "  mathsol script.msol    : Executes code in script.msol file\n";
  std::cout << "  mathsol -c 1000 - 7    : Executes the expression after the -c argument\n";
}

// Version information [-V, --version]
void printVersion() {
  std::cout << "MathSol version " << VERSION << "\n";
}

// Interactive mode [default]
void runInteractiveMode() {
  Lexer lex = Lexer();

  std::cout << "mathsol> ";
  std::string input;
  while (std::getline(std::cin, input)) {
    // Здесь будет реализован ваш интерпретатор
    std::vector<Token> tokens = lex.tokenize(input);
    for (Token t : tokens) std::cout << t << " ";
    std::cout << "\n";
    std::cout << "mathsol> ";
  }
}

// Execute expression [-c, --command]
void runCommand(const std::string& command) {
  Lexer lex = Lexer();
  std::vector<Token> tokens = lex.tokenize(command);
  for (Token t : tokens) std::cout << t << " ";
  std::cout << "\n";
}

// Execute code from file [filepath]
void runFile(const std::string& fileName) {
  std::ifstream file(fileName);
  if (!file.is_open()) {
    std::cerr << "Error: cant open file " << fileName << "\n";
    return;
  }
  
  Lexer lex = Lexer();
  std::string line;
  while (std::getline(file, line)) {
    std::cout << line << "\n";
    std::vector<Token> tokens = lex.tokenize(line);
    for (Token t : tokens) std::cout << t << " ";
    std::cout << "\n";
  }
  file.close();
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    // Starts interactive mode without arguments
    runInteractiveMode();
  } else if (argc == 2) {
    std::string arg = argv[1];
    if (arg == "-h" || arg == "--help") {
      printHelp();
    } else if (arg == "-v" || arg == "--version") {
      printVersion();
    } else {
      // Файл для выполнения
      runFile(arg);
    }
  } else {
    std::string arg = argv[1];
    if (arg == "-c" || arg == "--command") {
      std::string args = "";
      for (int i = 2; i < argc; i++) {
        args += argv[i];
        if (i < argc - 1) args += " ";
      }
      runCommand(args);
    }
    else {
      std::cerr << "Error: wrong argument format\n";
      printHelp();
    }
  }
  return 0;
}
