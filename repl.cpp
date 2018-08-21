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

int main(int argc, char* argv[]) {
    char* src, * src_start;
    Lexer lexer;

    while (true) {
        string input;
        getline(cin, input);

        if (input == "exit") {
            exit(EXIT_SUCCESS);
        }

        unordered_map<string, Identifier> symbol_table;
        vector<Token> tokens;

        src = new char[input.size() + 1];
        src_start = src;

        std::copy(input.begin(), input.end(), src);
        src[input.size()] = EOF;

        lexer.tokenize(src, tokens, symbol_table);

        Parser parser(tokens, symbol_table);

        int result = parser.parse();

        cout << result << endl;

        delete src_start;
    }
}