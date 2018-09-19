#include "Lexer.h"
#include "Parser.h"
#include "Evaluator.h"
#include "EvalResult.h"
#include <iostream>

bool isInputForLoop(const std::string& input) {
    return input.find("for") != std::string::npos;
}

bool isInputIfStmt(const std::string& input) {
    return input.find("if") != std::string::npos;
}

std::string readCompoundStatement(const std::string& input) {
    std::string stmt = input;

    if (stmt[stmt.size() - 1] == '}') {
        stmt.push_back('\n');
        return stmt;
    }

    std::string currentInput;
    while (true) {
        getline(std::cin, currentInput);

        if (std::cin.eof()) {
            exit(EXIT_SUCCESS);
        }

        if (currentInput[currentInput.size() - 1] == '}') {
            stmt += currentInput;
            stmt.push_back('\n');
            break;
        }

        if (isInputIfStmt(currentInput) || isInputForLoop(currentInput)) {
            stmt += readCompoundStatement(currentInput);
        } else if (currentInput.empty()) {
            continue;
        } else {
            currentInput.push_back('\n');
            stmt += currentInput;
        }
    }

    return stmt;
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
            if (isInputIfStmt(input) || isInputForLoop(input)) {
                input = readCompoundStatement(input);
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