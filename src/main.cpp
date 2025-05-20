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
    std::vector<Token> tokens = lex.tokenize(input);
    
    // Show tokens if requested
    if (showTokens) {
      for (Token t : tokens) std::cout << t << " ";
      std::cout << "\n";
    }
    
    // Show parse tree if requested
    if (showParseTree) {
      // TODO: parse tree implementation
      std::cout << "Parse tree will be implemented later\n";
    }
    std::cout << "mathsol> ";
  }
}

// Execute expression [-c, --command]
void runCommand(const std::string& command) {
  Lexer lex = Lexer();
  std::vector<Token> tokens = lex.tokenize(command);
  
  // Show tokens if requested
  if (showTokens) {
    for (Token t : tokens) std::cout << t << " ";
    std::cout << "\n";
  }
  
  // Show parse tree if requested
  if (showParseTree) {
    // TODO: parse tree implementation
    std::cout << "Parse tree will be implemented later\n";
  }
  
  // TODO: Execute when parser is implemented
  // Parser parser = Parser();
  // parser.parse(tokens);
}

// Execute code from file [filepath]
void runFile(const std::string& fileName) {
  std::ifstream file(fileName);
  if (!file.is_open()) {
    std::cerr << "Error: cant open file " << fileName << "\n";
    return;
  }
  
  Lexer lex = Lexer();
  //Parser parser = Parser();
  
  // Reset lexer state for fresh processing
  lex.reset();
  
  // Pre-allocate token vector
  std::vector<Token> allTokens;
  allTokens.reserve(1000); // Initial capacity
  
  // Buffer for reading chunks
  char buffer[CHUNK_BUFFER_SIZE];
  bool isEOF = false;
  
  // Line counter for display
  int lineCount = 1;
  std::string currentLine;
  
  // Process file in chunks
  while (!isEOF) {
    // Read next chunk
    file.read(buffer, CHUNK_BUFFER_SIZE - 1);
    std::streamsize bytesRead = file.gcount();
    buffer[bytesRead] = '\0'; // Null-terminate the buffer
    
    // Check if this is the last chunk
    isEOF = file.eof();
    
    // Process this chunk
    std::string chunk(buffer, bytesRead);
    std::vector<Token> chunkTokens = lex.tokenize(chunk, isEOF);
    
    // Extract and display lines for readability
    size_t lineStart = 0;
    for (size_t i = 0; i < chunk.size(); i++) {
      if (chunk[i] == '\n' || i == chunk.size() - 1) {
        // Complete the current line
        currentLine += chunk.substr(lineStart, i - lineStart + (i == chunk.size() - 1 ? 1 : 0));
        std::cout << currentLine << std::endl;
        
        // Reset for next line
        currentLine.clear();
        lineStart = i + 1;
        lineCount++;
      }
    }
    
    // Handle incomplete line at end of chunk
    if (lineStart < chunk.size()) {
      currentLine = chunk.substr(lineStart);
    }
    
    // Add tokens to our collection
    allTokens.insert(allTokens.end(), chunkTokens.begin(), chunkTokens.end());
  }
  
  // Get any final tokens including EOF
  if (isEOF) {
    std::vector<Token> finalTokens = lex.eof();
    allTokens.insert(allTokens.end(), finalTokens.begin(), finalTokens.end());
  }
  
  file.close();
  
  // Show tokens if requested
  if (showTokens) {
    for (const Token& t : allTokens) std::cout << t << " ";
    std::cout << "\n";
  }
  
  // Parse all tokens once they're collected (when parser is ready)
  // Parser parser = Parser();
  // parser.parse(allTokens);
  
  // Show parse tree if requested
  if (showParseTree) {
    // TODO: parse tree implementation
    std::cout << "Parse tree will be implemented later\n";
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
