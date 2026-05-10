#include <iostream>
#include <string>

#include "FileReader.h"
#include "LexerApplication.h"

int main(int argc, char* argv[]) {
    const std::string inputPath = argc > 1 ? argv[1] : "test.txt";

    pascal_lexer::FileReader fileReader;
    pascal_lexer::LexerApplication app(fileReader);

    return app.run(inputPath, std::cout, std::cerr);
}