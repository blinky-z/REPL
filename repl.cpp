#include "Lexer.h"
#include "Parser.h"
#include "Evaluator.h"
#include "EvalResult.h"
#include <iostream>

std::string readIfStmt(const std::string& input) {
    std::string ifStmt = input;

    std::string currentInput;
    while (!currentInput.empty()) {
        getline(std::cin, currentInput);

        if (std::cin.eof()) {
            exit(EXIT_SUCCESS);
        }

        ifStmt += currentInput;
    }

    return ifStmt;
}

std::string readForLoop(const std::string& input) {
    std::string forLoop = input;

    std::string currentInput;
    while (!currentInput.empty()) {
        getline(std::cin, currentInput);

        if (std::cin.eof()) {
            exit(EXIT_SUCCESS);
        }

        forLoop += currentInput;
    }

    return forLoop;
}

bool isInputForLoop(const std::string& input) {
    return input.find("for") != std::string::npos;
}

bool isIfStmt(const std::string& input) {
    return input.find("if") != std::string::npos;
}

void printResult(const EvalResult& result) {
    IdentifierValueType::ValueType resultType = result.getResultType();

    if (resultType == IdentifierValueType::Number) {
        std::cout << result.getResultDouble() << std::endl;
    } else if (resultType == IdentifierValueType::Bool) {
        std::cout << (result.getResultBool() ? "true" : "false") << std::endl;
    } else {
        std::cout << result.getResultString() << std::endl;
    }
}

int main() {
    Lexer lexer;
    Parser parser;
    Evaluator evaluator;

    while (true) {
        std::string input;
        getline(std::cin, input);

        if (std::cin.eof()) {
            exit(EXIT_SUCCESS);
        }

        if (input.size() != 0) {
            if (isInputForLoop(input)) {
                input = readForLoop(input);
            } else if (isIfStmt(input)) {
                input = readIfStmt(input);
            }

            input.push_back(EOF);

            const TokenContainer& tokens = lexer.tokenize(input);
            ASTNode* root = parser.parse(tokens);
            EvalResult result = evaluator.Evaluate(root);

            if (!result.isError()) {
                printResult(result);
            } else {
                throw std::runtime_error(result.error.what());
            }

            delete root;
        }
    }
}