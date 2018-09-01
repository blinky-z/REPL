#include "Lexer.h"
#include "Parser.h"
#include "Evaluator.h"
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

            std::string result = evaluator.Evaluate(root);

            // check if result is number
            if (result.find_first_not_of(".-0123456789") == std::string::npos) {
                std::cout << std::stod(result) << std::endl;
            } else {
                std::cout << result << std::endl;
            }

            delete root;
        }
    }
}