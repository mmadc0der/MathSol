# src/lexer/CMakeLists.txt
add_library(lexer
    src/lexer.cpp
    src/token.cpp
)

# Указываем, что заголовочные файлы находятся в include
target_include_directories(lexer PUBLIC include)