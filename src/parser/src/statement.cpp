#include "statement.hpp"

template<typename T>
T ExpressionStatement<T>::execute() const {  // TODO: implement expression types handling here
    return expression_.evaluate();
}