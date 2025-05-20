#include "literal.hpp"

template<typename T>
T Literal<T>::value() {
  if (not Literal<T>::value_) Literal<T>::value_ = parse_(Literal<T>::sourse_);
  return Literal<T>::value_;
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
