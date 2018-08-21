cmake_minimum_required(VERSION 3.12)
project(REPL)

set(CMAKE_CXX_STANDARD 11)

add_executable(REPL
        Token.h Identifier.h Lexer.cpp Lexer.h Parser.cpp Parser.h repl.cpp
        )
