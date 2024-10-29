#include "literal.hpp"

template<typename T>
T Literal<T>::value() {
  if (!std::is_null_pointer(value_)) value_ = parse_(sourse_)
  return value_;
}

template<>
std::string Literal<std::string>::parse_(const std::string& source) {
  if (source.size() <= 2) return "";
  else return source.substr(1, source.size() - 2); // ' or " symbols strip
}

template<>
double Literal<double>::parse_(const std::string& source) {
  return std::stod(source); // Преобразуем строку в double, если T — это double
}
