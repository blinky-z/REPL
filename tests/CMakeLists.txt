cmake_minimum_required(VERSION 3.12)
project(tests)

set(CMAKE_CXX_STANDARD 11)

add_executable(tests
#        src files
        ../Token.h ../Identifier.h
        ../Lexer.cpp ../Lexer.h
        ../Parser.cpp ../Parser.h
        ../AnalysisContainer.h ../AnalysisContainer.cpp
        ../ASTNode.h
        ../Evaluator.h ../Evaluator.cpp

#        ------------------------
#        tests
#        include lib to evaluate string math expressions
        tinyexpr.h tinyexpr.c

        provide_catch_main.cpp
        LexerTests.cpp
        EvaluatorTests.cpp
        )
