#include "token.hpp"

// Пустая реализация, т.к. класс Token не требует дополнительной реализации
std::ostream& operator << (std::ostream& outs, const Token& t) { return outs << "[" << nameTT[static_cast<int>(t.getType())] << ": " << t.getValue() << "]"; }
