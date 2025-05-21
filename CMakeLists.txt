cmake_minimum_required(VERSION 3.10)

project(mathsol)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Включаем общий каталог с заголовками
include_directories(src/lexer/include src/parser/include)

# Рекурсивно добавляем подкаталоги (модули)
add_subdirectory(src/lexer)
add_subdirectory(src/parser)

# Объявляем исполняемый файл и связываем с ним модули
add_executable(mathsol src/main.cpp)
target_link_libraries(mathsol lexer parser)
