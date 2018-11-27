cmake_minimum_required(VERSION 3.12)
project(LexerTests)
project(EvaluatorTests)
project(SemanticAnalyzerTests)
project(BashGeneratorTests)

set(CMAKE_CXX_STANDARD 11)

add_executable(EvaluatorTests
#        src files
        ../Token.h ../Identifier.h ../ASTNode.h
        ../Lexer.cpp ../Lexer.h
        ../Parser.cpp ../Parser.h
        ../Evaluator.h ../Evaluator.cpp
        ../SymbolTable.h ../SymbolTable.cpp
        ../TokenContainer.h ../TokenContainer.cpp
        ../EvalResult.cpp ../EvalResult.h
        ../SemanticAnalyzer.h ../SemanticAnalyzer.cpp
        ../SemanticAnalysisResult.h ../SemanticAnalysisResult.cpp
#        ------------------------
#        tests
#        include lib to evaluate string math expressions
        tinyexpr.h tinyexpr.c

        provide_catch_main.cpp
        LexerTests.cpp
        EvaluatorTests.cpp
        )

add_executable(SemanticAnalyzerTests
        #        src files
        ../Token.h ../Identifier.h ../ASTNode.h
        ../Lexer.cpp ../Lexer.h
        ../Parser.cpp ../Parser.h
        ../SymbolTable.h ../SymbolTable.cpp
        ../TokenContainer.h ../TokenContainer.cpp
        ../SemanticAnalyzer.h ../SemanticAnalyzer.cpp
        ../SemanticAnalysisResult.h ../SemanticAnalysisResult.cpp
        #        ------------------------
        #        tests

        provide_catch_main.cpp
        SemanticAnalyzerTests.cpp
        )

add_executable(LexerTests
        #        src files
        ../Token.h ../Identifier.h ../ASTNode.h
        ../TokenContainer.h ../TokenContainer.cpp
        ../Lexer.cpp ../Lexer.h
        ../SymbolTable.h ../SymbolTable.cpp
        #        ------------------------
        #        tests

        provide_catch_main.cpp
        LexerTests.cpp
        )

add_executable(BashGeneratorTests
        #        src files
        ../Token.h ../Identifier.h ../ASTNode.h
        ../Lexer.cpp ../Lexer.h
        ../Parser.cpp ../Parser.h
        ../SymbolTable.h ../SymbolTable.cpp
        ../TokenContainer.h ../TokenContainer.cpp
        ../SemanticAnalyzer.h ../SemanticAnalyzer.cpp
        ../SemanticAnalysisResult.h ../SemanticAnalysisResult.cpp
        ../BashGenerator.h ../BashGenerator.cpp
        #        ------------------------
        #        tests

        provide_catch_main.cpp
        BashGeneratorTests.cpp
        )