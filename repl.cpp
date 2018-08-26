#include "Lexer.h"
#include "Parser.h"
#include "Identifier.h"
#include "Token.h"
#include "ASTNode.h"
#include "Evaluator.h"
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <unordered_map>

int main() {
    Evaluator evaluator;

    while (true) {
        string input;
        getline(cin, input);

        if (input == "exit") {
            exit(EXIT_SUCCESS);
        }
        input.push_back(EOF);

        Lexer lexer;
        AnalysisContainer data = lexer.tokenize(input);

        Parser parser(data);
        ASTNode* root = parser.parse();

        double result = evaluator.Evaluate(root);
        input.pop_back();
        cout << input << " = " << result << endl;

        delete root;
    }
}