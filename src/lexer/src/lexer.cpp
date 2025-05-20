#include "lexer.hpp"
#include <set>

Lexer::Lexer() : pending_token_value_("") {}

std::pair<Token, bool> Lexer::scan_next_(const std::string& chunk, size_t& iter) {
  // pending_ обрабатывается в tokenize путем конкатенации
  size_t last_index = chunk.size() - 1;
  size_t local_index = 0;
  std::set<int> valid_tokens;
  std::string value = "";
  bool separated = false;

  if (iter >= chunk.size()) return { Token(TokenType::PENDING, "PENDING_EOF_REACHED"), false }; // Используем PENDING, если чанк закончился
  
  // Skip whitespace
  while (iter < chunk.size() && (chunk[iter] == ' ' || chunk[iter] == '\t')) iter++;
  if (iter >= chunk.size()) {
    return { Token(TokenType::PENDING, "PENDING_AFTER_WHITESPACE"), false }; // Закончился чанк после пробелов
  };
  value += chunk[iter];

  // Check for newline character
  if (chunk[iter] == '\n') {
    iter++;
    return { Token(TokenType::EOL, "EOL"), true };
  }

  for (int i = 0; i < static_cast<int>(TokenType::ERROR); i++) {
    if (valueTT[i][local_index] == value[local_index]) valid_tokens.insert(i);
  }

  iter++;
  local_index++;
  while (iter <= last_index) {
    if (chunk[iter] == ' ' || chunk[iter] == '\t') {
      separated = true;
      break;
    }
    value += chunk[iter];

    std::set<int> tokens_to_erase;
    for (int i : valid_tokens) {
      if (valueTT[i].size() <= local_index || valueTT[i][local_index] != value[local_index]) tokens_to_erase.insert(i);
      //else if (valueTT[i][local_index] != value[local_index]) tokens_to_erase.insert(i); // Объединено выше
      else if (valueTT[i][local_index] != value[local_index]) tokens_to_erase.insert(i);
      //else continue; // Лишнее
    }
    if (valid_tokens.size() == tokens_to_erase.size() && !value.empty()) { 
      value.pop_back(); // Убираем последний символ, который сделал все невалидным
      // iter--; // iter должен указывать на символ *после* успешно разобранной части токена.
      // Этот декремент приводил к зацикливанию на односимвольных токенах.
      break; 
    }
    for (int t : tokens_to_erase) valid_tokens.erase(t);

    if (valid_tokens.size() == 1) {
      iter++;
      std::string target_value = valueTT[*(valid_tokens.begin())];
      // iter уже инкрементирован, value содержит потенциальное начало токена
      std::pair<Token, bool> scanned = scan_const_or_id_(chunk, iter, value, true);
      if (target_value == scanned.first.getValue()) return { Token(static_cast<TokenType>(*(valid_tokens.begin())), value), scanned.second };
      else return scanned;
    }
    iter++; local_index++;
  }

  if (valid_tokens.size() >= 1) {
    for (int i : valid_tokens) {
      if (valueTT[i] == value) return { Token(static_cast<TokenType>(i), value), separated };
    }
  }
  // iter НЕ инкрементирован после последнего добавления в value, если мы здесь
  // value содержит то, что не подошло ни под один valueTT
  return scan_const_or_id_(chunk, iter, value, false);
}


std::pair<Token, bool> Lexer::scan_const_or_id_(const std::string& chunk, size_t& iter, std::string& value, bool initial_char_consumed) {
  TokenType tt = TokenType::ERROR;
  if (value[0] == '#') {                                      // DELIMETER_COMMENT
    tt = TokenType::DELIMETER_COMMENT;
    while (iter < chunk.size()) {
      if (chunk[iter] != '\n') value += chunk[iter];
      else {
        // iter--; // Не нужно, так как EOL будет обработан на следующей итерации scan_next_
        return { Token(TokenType::DELIMETER_COMMENT, value), true }; // Комментарий всегда отделен (EOL или EOF)
      }
      iter++;
    }
  } else if (value[0] >= '0' && value[0] <= '9') {            // CONSTANT_NUM
    tt = TokenType::CONSTANT_NUM;
    bool doted = false;
    // Первый символ уже в value
    // iter указывает на следующий символ для проверки
    while (iter < chunk.size()) {
      if (chunk[iter] >= '0' && chunk[iter] <= '9') value += chunk[iter];
      else if (chunk[iter] == '.' && !doted) {
        value += chunk[iter];
        doted = true;
      }
      else { 
        // Встретили символ, не являющийся частью числа. Завершаем число.
        // separated будет true, если chunk[iter] это пробел или табуляция
        bool separated_after = (iter < chunk.size() && (chunk[iter] == ' ' || chunk[iter] == '	'));
        return { Token(TokenType::CONSTANT_NUM, value), separated_after }; 
      }
      iter++;
    }
  } else if (value[0] == '\'' || value[0] == '\"') {         // CONSTANT_STRING
    tt = TokenType::CONSTANT_STRING;
    char opener = value[0];
    // Первый символ уже в value (открывающая кавычка)
    // iter указывает на следующий символ для проверки
    while (iter < chunk.size()) {
      if (chunk[iter] != opener) value += chunk[iter];
      else if (chunk[iter] == '\n') {
        // iter--; // Не нужно, ошибка синтаксиса, перевод строки внутри строки без экранирования
        return { Token(TokenType::ERROR, "Wrong syntax (newline in string): \"" + value + "\""), true };
      }
      else {
        value += chunk[iter];
        iter++;
        return { Token(TokenType::CONSTANT_STRING, value), true };
      }
      iter++;
    }
  } else if (value[0] == '_'                                  // IDENTIFIER
    || (value[0] >= 'A' && value[0] <= 'Z') 
    || (value[0] >= 'a' && value[0] <= 'z')) { 
    tt = TokenType::IDENTIFIER;
    // Первый символ уже в value
    // iter указывает на следующий символ для проверки
    while (iter < chunk.size()) {
      if (chunk[iter] == '-' 
        || chunk[iter] == '_' 
        || (chunk[iter] >= 'A' && chunk[iter] <= 'Z') 
        || (chunk[iter] >= 'a' && chunk[iter] <= 'z')
        || (chunk[iter] >= '0' && chunk[iter] <= '9')) 
      value += chunk[iter];
      else { 
        // Встретили символ, не являющийся частью идентификатора. Завершаем идентификатор.
        bool separated_after = (iter < chunk.size() && (chunk[iter] == ' ' || chunk[iter] == '	'));
        return { Token(TokenType::IDENTIFIER, value), separated_after }; 
      }
      iter++;
    }
  }
  // Если цикл завершился из-за конца чанка (iter == chunk.size())
  // то токен считается завершенным, но не обязательно отделенным пробелом (separated = false)
  return { Token(tt, value), false };  
}

std::vector<Token> Lexer::tokenize(const std::string& source_chunk) {
  std::string current_input = pending_token_value_; // Начинаем с остатка от предыдущего раза
  pending_token_value_.clear(); // Сбрасываем pending перед новой обработкой
  current_input += source_chunk; // Добавляем новый кусок

  if (current_input.empty()) return {};

  std::vector<Token> tokens;
  size_t iter = 0;
  Token current_token = Token(TokenType::ERROR);
  bool current_token_separated = false;

  // Для отслеживания последнего токена, добавленного в `tokens` в этом чанке
  Token last_added_token_in_chunk = Token(TokenType::ERROR);
  bool last_added_token_separated = false;
  bool token_was_added_in_loop = false;

  do {
    std::pair<Token, bool> scanned = scan_next_(current_input, iter);
    current_token = scanned.first;
    current_token_separated = scanned.second;
    if (current_token.getType() == TokenType::PENDING) {
      if (iter < current_input.size()) {
        pending_token_value_ = current_input.substr(iter);
      }
      break; 
    }

    if (current_token.getType() == TokenType::ERROR && current_token.getValue().empty() && iter >= current_input.size()) {
      // Это может быть ситуация, когда scan_next_ ничего не нашел в пустом остатке current_input
      // Не добавляем такой ERROR токен и выходим.
      break;
    }

    tokens.push_back(current_token);
    last_added_token_in_chunk = current_token;
    last_added_token_separated = current_token_separated;
    token_was_added_in_loop = true;

    if (current_token.getType() == TokenType::ERROR) {
      break;
    }

  } while (current_token.getType() != TokenType::ERROR && 
           current_token.getType() != TokenType::_EOF &&
           iter < current_input.size());

  // После цикла проверяем, не нужно ли последний добавленный токен сделать pending
  if (token_was_added_in_loop && iter >= current_input.size() && 
      last_added_token_in_chunk.getType() != TokenType::ERROR &&
      last_added_token_in_chunk.getType() != TokenType::_EOF &&
      last_added_token_in_chunk.getType() != TokenType::ERROR &&
      !last_added_token_separated) {
    pending_token_value_ = last_added_token_in_chunk.getValue();
    // Убираем последний токен из списка, так как он теперь pending
    // Но только если он действительно там последний (на случай если токен был, но не добавился из-за ошибки или EOF)
    if (!tokens.empty() && tokens.back().getValue() == last_added_token_in_chunk.getValue() && tokens.back().getType() == last_added_token_in_chunk.getType()) {
      tokens.pop_back(); 
    }
  }

  return tokens;
}

// Ensure Lexer::eof() starts on a new line after tokenize's closing brace

std::vector<Token> Lexer::eof() {
  std::vector<Token> final_tokens;
  if (!pending_token_value_.empty()) {
    size_t temp_iter = 0;
    std::pair<Token, bool> scanned = scan_next_(pending_token_value_, temp_iter);

    // Если pending_token_value_ был полностью разобран как один токен
    if (scanned.first.getType() != TokenType::ERROR && 
      scanned.first.getType() != TokenType::PENDING && 
      temp_iter == pending_token_value_.size()) {
      final_tokens.push_back(scanned.first);
    } 
    // Если scan_next_ вернул PENDING, но что-то распарсил (например, это незавершенный идентификатор)
    // или если он вернул валидный токен, но не всю строку pending_token_value_
    else if (scanned.first.getType() != TokenType::ERROR && !scanned.first.getValue().empty()) {
      // Если scan_next_ вернул PENDING с непустым значением, которое не является стандартным сообщением PENDING.
      // Либо если он вернул обычный токен, но обработал не всю строку pending.
      // В таких случаях, предполагаем, что это идентификатор или число, если возможно.
      // Эта логика может потребовать уточнений.
      if (scanned.first.getType() == TokenType::PENDING) {
        if (scanned.first.getValue() != "PENDING_EOF_REACHED" && 
            scanned.first.getValue() != "PENDING_AFTER_WHITESPACE" && 
            !scanned.first.getValue().empty()) {
          // Пытаемся распознать как константу или идентификатор
          std::string val_to_check = scanned.first.getValue();
          size_t check_iter = 0; // iter для scan_const_or_id
          // Передаем false для initial_char_consumed, так как scan_next_ вернул PENDING
          // и мы передаем ему полный остаток value.
          std::pair<Token, bool> const_id_scan = scan_const_or_id_(val_to_check, check_iter, val_to_check, false);
          if(const_id_scan.first.getType() != TokenType::ERROR && check_iter == val_to_check.size()) {
            final_tokens.push_back(const_id_scan.first);
          } else {
            final_tokens.push_back(Token(TokenType::ERROR, "Unfinished pending at EOF: " + pending_token_value_));
          }
        } else if (!pending_token_value_.empty()) { // scan_next_ вернул стандартный PENDING, но строка была
          final_tokens.push_back(Token(TokenType::ERROR, "Unprocessed pending at EOF: " + pending_token_value_));
        }
      } else { // scan_next_ вернул валидный токен, но не всю строку pending_token_value_
        final_tokens.push_back(scanned.first); // Добавляем то, что удалось распознать
        if (temp_iter < pending_token_value_.size()) { // Если что-то осталось
          final_tokens.push_back(Token(TokenType::ERROR, "Extra chars after pending at EOF: " + pending_token_value_.substr(temp_iter)));
        }
      }
    } else if (!pending_token_value_.empty()) { // scan_next_ вернул ERROR или PENDING (без данных)
      final_tokens.push_back(Token(TokenType::ERROR, "Unable to process pending at EOF: " + pending_token_value_));
    }
  }
  pending_token_value_.clear();
  final_tokens.push_back(Token(TokenType::_EOF, "EOF"));
  return final_tokens;
}