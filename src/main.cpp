#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "lexer/include/lexer.hpp"

// Функция для вывода справки
void printHelp() {
  std::cout << "MathSol: интерпретатор для языка MathSol\n";
  std::cout << "Использование:\n";
  std::cout << "  mathsol [файл]  # выполнить код из файла\n";
  std::cout << "  mathsol --help  # вывод справки\n";
  std::cout << "  mathsol --version  # вывод версии\n";
}

// Функция для вывода версии
void printVersion() {
  std::cout << "MathSol версия 1.0.2\n";
}

// Функция для запуска интерактивного режима
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

// Функция для выполнения кода из файла
void runFile(const std::string& fileName) {
  std::ifstream file(fileName);
  if (!file.is_open()) {
    std::cerr << "Ошибка: не удалось открыть файл " << fileName << "\n";
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
    // Без аргументов - запуск интерактивного режима
    runInteractiveMode();
  } else if (argc == 2) {
    std::string arg = argv[1];
    if (arg == "--help") {
      printHelp();
    } else if (arg == "--version") {
      printVersion();
    } else {
      // Файл для выполнения
      runFile(arg);
    }
  } else {
    std::cerr << "Ошибка: неправильное количество аргументов\n";
    printHelp();
  }
  return 0;
}

