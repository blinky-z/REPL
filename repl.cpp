#include "Lexer.h"
#include "Parser.h"
#include "Evaluator.h"
#include <iostream>

int main() {
    Lexer lexer;
    Parser parser;
    Evaluator evaluator;

    while (true) {
        std::string input;
        getline(std::cin, input);

        if (input == "exit") {
            exit(EXIT_SUCCESS);
        }
        input.push_back(EOF);

        const TokenContainer& tokens = lexer.tokenize(input);

        ASTNode* root = parser.parse(tokens);

        std::string result = evaluator.Evaluate(root);
        input.pop_back();

        // check if result is number
        if (result.find_first_not_of(".-0123456789") == std::string::npos) {
            std::cout << std::stod(result) << std::endl;
        } else {
            std::cout << result << std::endl;
        }

        delete root;
    }
}