#include "global.hpp"

// Global flags for command-line options
bool showTokens = false;    // Enable token output
bool showParseTree = false; // Enable parse tree output

// Help information [-h, --help]
void printHelp() {
  std::cout << "MathSol v" << VERSION << " mathsol language interpreter\n";
  std::cout << "(C) 2024-now madc0der\n\n";
  std::cout << "Usage:\n  mathsol [option] [file]\n\n";
  std::cout << "Options:\n";
  std::cout << "  -c, --command  : program passed in as a string\n";
  std::cout << "  -h, --help     : display this help information\n";
  std::cout << "  -V, --version  : display version information\n";
  std::cout << "  -t, --tokens   : show tokens\n";
  std::cout << "  -T, --tree     : show parse tree\n\n";
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
    // Tokenize the input
    std::vector<Token> tokens = lex.tokenize(input + "\n");
    
    // Show tokens if requested
    if (showTokens) {
      for (Token t : tokens) std::cout << t << " ";
      std::cout << "\n";
    }
    
    // Парсинг и вывод дерева (если нужно)
    if (!tokens.empty() && !(tokens.size() == 1 && tokens[0].getType() == TokenType::_EOF)) {
      Parser parser(tokens); // Создаем парсер с полученными токенами
      std::vector<std::unique_ptr<IStatement>> statements = parser.parse();

      // TODO: Добавить проверку parser.hasError() когда метод будет доступен
      // if (parser.hasError()) {
      //     std::cerr << "Parsing errors occurred.\n";
      // }

      if (showParseTree /*&& !parser.hasError()*/) {
        if (!statements.empty() || (tokens.size() > 1 || (tokens.size()==1 && tokens[0].getType() != TokenType::_EOF) )) {
          AstPrinter printer;
          std::cout << "--- AST Tree ---\n";
          std::cout << printer.print(statements); // Печатаем дерево
          std::cout << "------------------\n";
        } /*else if (parser.hasError())*/ else if (false) { // Временно, пока нет parser.hasError()
          std::cout << "Parsing resulted in errors, no AST to show.\n";
        } else {
          std::cout << "Input resulted in no statements to display in AST.\n";
        }
      }

      // TODO: Здесь будет вычисление выражений из statements
      // for (const auto& stmt : statements) {
      //     if (stmt /*&& !parser.hasError()*/) {
      //         // stmt->execute(env); // или evaluate и printValue
      //     }
      // }
    }
    std::cout << "mathsol> ";
  }

  // After the loop (e.g., on EOF), process any remaining pending tokens and get EOF
  std::vector<Token> final_tokens = lex.eof();
  if (showTokens && !final_tokens.empty()) {
    for (Token t : final_tokens) std::cout << t << " ";
    std::cout << "\n";
  }
  // TODO: If a parser were here, it would also need to process these final_tokens.
}

// Execute expression [-c, --command]
void runCommand(const std::string& command) {
  Lexer lex = Lexer();
  std::vector<Token> tokens = lex.tokenize(command);
  
  // Process any remaining pending tokens and get EOF
  std::vector<Token> eof_tokens = lex.eof();
  tokens.insert(tokens.end(), eof_tokens.begin(), eof_tokens.end());

  // Show tokens if requested
  if (showTokens) {
    for (Token t : tokens) std::cout << t << " ";
    std::cout << "\n";
  }
  
  // Парсинг и вывод дерева (если нужно)
  if (!tokens.empty() && !(tokens.size() == 1 && tokens[0].getType() == TokenType::_EOF)) {
    Parser parser(tokens);
    std::vector<std::unique_ptr<IStatement>> statements = parser.parse();

    // TODO: Добавить проверку parser.hasError()
    if (showParseTree /*&& !parser.hasError()*/) {
      if (!statements.empty() || (tokens.size() > 1 || (tokens.size()==1 && tokens[0].getType() != TokenType::_EOF) )) {
        AstPrinter printer;
        std::cout << "--- AST Tree ---\n";
        std::cout << printer.print(statements);
        std::cout << "------------------\n";
      } /*else if (parser.hasError())*/ else if (false) { // Временно, пока нет parser.hasError()
        std::cout << "Parsing resulted in errors, no AST to show.\n";
      } else {
        std::cout << "Input resulted in no statements to display in AST.\n";
      }
    }

    // TODO: Здесь будет вычисление выражений из statements
    // for (const auto& stmt : statements) { ... }
  }
}

// Execute code from file [filepath]
void runFile(const std::string& fileName) {
  std::ifstream file(fileName);
  if (!file.is_open()) {
    std::cerr << "Error: cant open file " << fileName << "\n";
    return;
  }
  
  Lexer lex = Lexer();
  
  std::vector<Token> allTokens;
  allTokens.reserve(256); // Initial capacity
  
  // Buffer for reading chunks
  char buffer[CHUNK_BUFFER_SIZE];
  
  // Process file in chunks for tokenisation
  while (!file.eof()) {
    // Read next chunk
    file.read(buffer, CHUNK_BUFFER_SIZE - 1);
    std::streamsize bytesRead = file.gcount();
    
    if (bytesRead == 0) {
      break; // No more data to process
    }
    
    buffer[bytesRead] = '\0'; // Null-terminate the buffer
    
    // Process this chunk
    std::string chunk(buffer, bytesRead);
    std::vector<Token> chunkTokens = lex.tokenize(chunk);
    
    // Add tokens to our collection
    allTokens.insert(allTokens.end(), chunkTokens.begin(), chunkTokens.end());
  }
  
  // Get final EOF token
  std::vector<Token> eof_tokens = lex.eof();
  allTokens.insert(allTokens.end(), eof_tokens.begin(), eof_tokens.end());
  
  file.close();
  
  // Show tokens if requested
  if (showTokens) {
    for (const Token& t : allTokens) std::cout << t << " ";
    std::cout << "\n";
  }
  
  // Debug: Check allTokens before parsing block
  std::cout << "DEBUG runFile: Before parsing block. allTokens.size() = " << allTokens.size() << std::endl;
  if (!allTokens.empty()) {
    std::cout << "DEBUG runFile: Last token type: " << allTokens.back() << std::endl;
  }

  // Parse all tokens once they're collected (when parser is ready)
  if (!allTokens.empty() && !(allTokens.size() == 1 && allTokens[0].getType() == TokenType::_EOF)) {
    Parser parser(allTokens);
    std::vector<std::unique_ptr<IStatement>> statements;
    try {
        std::cout << "Attempting to parse tokens...\n"; // Отладочный вывод
        statements = parser.parse();
        std::cout << "Parser returned " << statements.size() << " statements\n";
    } catch (const std::exception& e) {
        std::cerr << "Exception caught during parsing in runFile: " << e.what() << '\n';
        // Можно добавить return или обработку, чтобы не продолжать с пустыми statements
        return; // Выходим из runFile при ошибке парсинга
    } catch (...) {
        std::cerr << "Unknown exception caught during parsing in runFile.\n";
        return; // Выходим из runFile при неизвестной ошибке
    }

    // TODO: Добавить проверку parser.hasError()
    if (showParseTree /*&& !parser.hasError()*/) {
      if (!statements.empty() || (allTokens.size() > 1 || (allTokens.size()==1 && allTokens[0].getType() != TokenType::_EOF) )) {
        AstPrinter printer;
        std::cout << "--- AST Tree ---\n";
        std::cout << printer.print(statements);
        std::cout << "------------------\n";
      } /*else if (parser.hasError())*/ else if (false) { // Временно, пока нет parser.hasError()
        std::cout << "Parsing resulted in errors, no AST to show.\n";
      } else {
        std::cout << "File content resulted in no statements to display in AST.\n";
      }
    }

    // TODO: Здесь будет вычисление выражений из statements
    // for (const auto& stmt : statements) { ... }
  } else if (showParseTree) {
    std::cout << "File is empty or resulted in no tokens to parse for AST.\n";
  }

}

// Process short argument sequence [-abc] and return true if the sequence contains option that requires parameters
bool processShortArgSequence(const std::string& argSequence, char& lastOption) {
  bool requiresParam = false;
  
  for (size_t i = 1; i < argSequence.size(); ++i) {
    char option = argSequence[i];
    lastOption = option;
    
    switch (option) {
      case 'h':
        printHelp();
        break;
      case 'V':
        printVersion();
        break;
      case 't':
        showTokens = true;
        break;
      case 'T':
        showParseTree = true;
        break;
      case 'c': // This option requires additional parameter
        requiresParam = true;
        break;
      default:
        std::cerr << "Error: unknown option -" << option << "\n";
        printHelp();
        break;
    }
  }
  
  return requiresParam;
}

// Process argument
bool processArgument(const std::string& arg, char& lastOption) {
  if (arg[0] == '-') {
    if (arg.size() > 2 && arg[1] == '-') {
      // Long option
      if (arg == "--help") {
        printHelp();
        return true;
      } else if (arg == "--version") {
        printVersion();
        return true;
      } else if (arg == "--tokens") {
        showTokens = true;
        return true;
      } else if (arg == "--tree") {
        showParseTree = true;
        return true;
      } else if (arg == "--command") {
        // For option -c additional arguments are required
        lastOption = 'c';
        return false;
      } else {
        std::cerr << "Error: unknown option " << arg << "\n";
        printHelp();
        return true;
      }
    } else if (arg.size() > 1) {
      if (arg == "-c") {
        // For option -c additional arguments are required
        lastOption = 'c';
        return false;
      } else {
        // Short argument sequence (-abc)
        bool requiresParam = processShortArgSequence(arg, lastOption);
        return !requiresParam; // Return false if sequence includes option requiring parameter
      }
    }
  }
  
  // Not option
  return false;
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    runInteractiveMode();
    return 0;
  }
  
  bool hasCommandOption = false;
  std::string commandArg;
  char lastOption = 0;
  int i = 1;
  
  while (i < argc) {
    std::string arg = argv[i];
    
    if (arg == "-c" || arg == "--command") {
      hasCommandOption = true;
      commandArg = arg;
      i++;
      break;
    } else if (arg[0] == '-') {
      if (processArgument(arg, lastOption)) {
        // Argument processed, continue
        i++;
      } else {
        // This is an option that requires arguments
        hasCommandOption = (lastOption == 'c');
        commandArg = arg;
        i++;
        break;
      }
    } else {
      // Found an argument that is not an option (file name)
      break;
    }
  }
  
  if (hasCommandOption) {
    // Command processing (-c, --command)
    std::string command = "";
    for (; i < argc; i++) {
      command += argv[i];
      if (i < argc - 1) command += " ";
    }
    
    if (command.empty()) {
      std::cerr << "Error: " << commandArg << " option requires an argument\n";
      return 1;
    }
    
    runCommand(command);
  } else if (i < argc) {
    // File execution
    runFile(argv[i]);
  } else {
    // No command or file, run interactive mode
    runInteractiveMode();
  }
  return 0;
}
