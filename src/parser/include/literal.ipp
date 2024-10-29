#pragma once

template<typename T>
T Literal<T>::value() {
  if (source_.empty()) {
    return value_;
  }
  return parse_(source_);
}

template<>
std::string Literal<std::string>::parse_(const std::string& source) {
  return source; // Возвращаем саму строку, если T — это std::string
}

template<>
double Literal<double>::parse_(const std::string& source) {
  return std::stod(source); // Преобразуем строку в double, если T — это double
}
