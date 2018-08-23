#include "Lexer.h"
#include "Parser.h"
#include "Identifier.h"
#include "Token.h"
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <unordered_map>

int main() {
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
        int result = parser.parse();

        cout << result << endl;
    }
}