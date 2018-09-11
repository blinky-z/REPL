#include "Lexer.h"
#include "Parser.h"
#include "Evaluator.h"
#include "EvalResult.h"
#include <iostream>

std::string readForLoop(const std::string input) {
    std::string forLoopInput = input;

    while (true) {
        std::string currentInput;
        getline(std::cin, currentInput);

        if (!currentInput.empty()) {
            forLoopInput += currentInput;
        } else {
            break;
        }
    }

    return forLoopInput;
}

bool isInputForLoop(const std::string& input) {
    return input.find("for") != std::string::npos;
}

void printResult(const EvalResult& result) {
    IdentifierValueType::ValueType resultType = result.getResultType();

    if (resultType == IdentifierValueType::Number) {
        std::cout << result.getResultDouble() << std::endl;
    } else if (resultType == IdentifierValueType::Bool) {
        std::cout << result.getResultBool() << std::endl;
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