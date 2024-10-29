# src/parser/CMakeLists.txt
add_library(parser
    src/expression.cpp
    src/statement.cpp
    src/parser.cpp
)

# Указываем, что заголовочные файлы находятся в include
target_include_directories(parser PUBLIC include)