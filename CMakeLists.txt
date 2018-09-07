cmake_minimum_required(VERSION 3.12)
project(REPL)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_subdirectory(tests)
add_executable(REPL
        repl.cpp
        Token.h Identifier.h ASTNode.h
        Lexer.cpp Lexer.h
        Parser.cpp Parser.h
        Evaluator.cpp Evaluator.h
        TokenContainer.cpp TokenContainer.h
        SymbolTable.cpp SymbolTable.h
        )