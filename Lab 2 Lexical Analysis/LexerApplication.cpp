#include "LexerApplication.h"

#include <exception>
#include <ostream>
#include <vector>

#include "PascalLexer.h"
#include "Token.h"

namespace pascal_lexer {

    LexerApplication::LexerApplication(const ISourceReader& sourceReader)
        : sourceReader_(sourceReader) {
    }

    int LexerApplication::run(
        const std::string& inputPath,
        std::ostream& output,
        std::ostream& error
    ) const {
        try {
            const std::string source = sourceReader_.readAllText(inputPath);

            PascalLexer lexer(source);
            const std::vector<Token> tokens = lexer.tokenize();

            for (const Token& token : tokens) {
                if (token.type == TokenType::EndOfFile) {
                    continue;
                }

                output << formatToken(token) << '\n';
            }

            return 0;
        }
        catch (const std::exception& exception) {
            error << exception.what() << '\n';
            return 1;
        }
    }

}