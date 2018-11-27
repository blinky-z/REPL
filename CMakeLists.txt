cmake_minimum_required(VERSION 3.12)
project(REPL)
project(Compiler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_subdirectory(sole)
add_subdirectory(tests)
add_executable(REPL
        repl.cpp
        Evaluator.cpp Evaluator.h
        Token.h Identifier.h ASTNode.h
        Lexer.cpp Lexer.h
        Parser.cpp Parser.h
        TokenContainer.cpp TokenContainer.h
        SymbolTable.cpp SymbolTable.h
        EvalResult.cpp EvalResult.h
        SemanticAnalysisResult.cpp SemanticAnalysisResult.h
        SemanticAnalyzer.cpp SemanticAnalyzer.h
        )

add_executable(Compiler
        compiler.cpp
        Token.h Identifier.h ASTNode.h
        Lexer.cpp Lexer.h
        Parser.cpp Parser.h
        TokenContainer.cpp TokenContainer.h
        SymbolTable.cpp SymbolTable.h
        BashGenerator.cpp BashGenerator.h
        SemanticAnalysisResult.cpp SemanticAnalysisResult.h
        SemanticAnalyzer.cpp SemanticAnalyzer.h
        )