#include "Lexer.h"
#include "Parser.h"
#include "Evaluator.h"
#include "EvalResult.h"
#include <iostream>

bool isInputForLoop(const std::string& input) {
    return input.find("for") != std::string::npos;
}

bool isIfStmt(const std::string& input) {
    return input.find("if") != std::string::npos;
}

std::string readIfStmt(const std::string& input) {
    std::string ifStmt = input;

    if (ifStmt[ifStmt.size() - 1] == '}') {
        ifStmt.push_back('\n');
        return ifStmt;
    }

    std::string currentInput;
    while (true) {
        getline(std::cin, currentInput);

        if (std::cin.eof()) {
            exit(EXIT_SUCCESS);
        }

        if (currentInput[currentInput.size() - 1] == '}') {
            ifStmt += currentInput;
            ifStmt.push_back('\n');
            break;
        }

        if (isIfStmt(currentInput)) {
            ifStmt += readIfStmt(currentInput);
        } else if (currentInput.empty()) {
            continue;
        } else {
            currentInput.push_back('\n');
            ifStmt += currentInput;
        }
    }
    return ifStmt;
}

std::string readForLoop(const std::string& input) {
    std::string forLoop = input;
    forLoop.push_back('\n');

    std::string currentInput;
    while (true) {
        getline(std::cin, currentInput);

        if (std::cin.eof()) {
            exit(EXIT_SUCCESS);
        }
        if (currentInput.empty()) {
            break;
        }

        currentInput += "\n";
        forLoop += currentInput;
    }

    return forLoop;
}

void printResult(const EvalResult& result, int tabCount = 0) {
    ValueType::Type resultType = result.getResultType();

    if (!result.isError()) {
        if (resultType == ValueType::Number) {
            std::cout << result.getResultDouble() << std::endl;
        } else if (resultType == ValueType::Bool) {
            std::cout << (result.getResultBool() ? "true" : "false") << std::endl;
        } else if (resultType == ValueType::Compound) {
            std::cout << "Block Statement Result:" << std::endl;
            tabCount++;
            for (const auto& currentResult : result.getResultBlock()) {
                for (int i = 0; i < tabCount; i++) {
                    std::cout << "\t";
                }
                printResult(currentResult, tabCount);
            }
        } else if (resultType == ValueType::String) {
            std::cout << result.getResultString() << std::endl;
        }
    } else {
        throw std::runtime_error(result.error.what());
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

            if (input.back() != '\n') {
                input.push_back('\n');
            }
            input.push_back(EOF);

            const TokenContainer& tokens = lexer.tokenize(input);
            ASTNode* root = parser.parse(tokens);
            EvalResult result = evaluator.Evaluate(root);

            printResult(result);

            delete root;
        }
    }
}