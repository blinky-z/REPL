cmake_minimum_required(VERSION 3.12)
project(tests)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_executable(tests
        ../Token.h ../Identifier.h
        ../Lexer.cpp ../Lexer.h
        ../Parser.cpp ../Parser.h
        ../AnalysisContainer.h ../AnalysisContainer.cpp
#        ------------------------
#        tests
        provide_catch_main.cpp
        LexerTests.cpp
        )
