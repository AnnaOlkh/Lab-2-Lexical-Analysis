#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "FileReader.h"
#include "PascalLexer.h"

using pascal_lexer::FileReader;
using pascal_lexer::PascalLexer;
using pascal_lexer::Token;
using pascal_lexer::TokenType;
using pascal_lexer::formatToken;

int main(int argc, char* argv[]) {
    const std::string inputPath = argc > 1 ? argv[1] : "test.txt";

    try {
        const std::string source = FileReader::readAllText(inputPath);
        PascalLexer lexer(source);
        const std::vector<Token> tokens = lexer.tokenize();

        for (const Token& token : tokens) {
            if (token.type == TokenType::EndOfFile) {
                continue;
            }
            std::cout << formatToken(token) << '\n';
        }

        return 0;
    }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
