cmake_minimum_required(VERSION 3.10)
project(mathsol)

# Включаем каталог с заголовками
include_directories(src/lexer/include)

# Добавляем исполняемый файл и исходники
add_executable(mathsol src/main.cpp src/lexer/src/token.cpp src/lexer/src/lexer.cpp)