#pragma once
#include <string>

template<typename T>
class ILiteral {
  public:
    T virtual value();
};

template<typename T>
class Literal : public ILiteral<T> {
  public:
    Literal();
    Literal(const std::string& source) : source_(source) {};
    
    T value() override;
  
  private:
    std::string source_;
    T value_;

    T parse_(const std::string& source);
};