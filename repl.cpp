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
    char* src, * src_start;

    while (true) {
        string input;
        getline(cin, input);

        if (input == "exit") {
            exit(EXIT_SUCCESS);
        }

        Lexer lexer;

        src = new char[input.size() + 1];
        src_start = src;

        std::copy(input.begin(), input.end(), src);
        src[input.size()] = EOF;

        AnalysisContainer data = lexer.tokenize(src);

        Parser parser(data);

        int result = parser.parse();

        cout << result << endl;

        delete src_start;
    }
}