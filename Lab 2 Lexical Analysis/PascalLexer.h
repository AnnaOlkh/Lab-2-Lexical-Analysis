#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "Token.h"

namespace pascal_lexer {

    class PascalLexer {
    public:
        explicit PascalLexer(std::string source);

        std::vector<Token> tokenize();

    private:
        std::string source_;
        std::size_t position_;
        std::size_t line_;
        std::size_t column_;

        static const std::unordered_set<std::string>& keywords();
        static const std::unordered_set<std::string>& wordOperators();

        bool isAtEnd() const;
        char peek(std::size_t offset = 0) const;
        char advance();
        bool match(char expected);

        void skipWhitespace();

        Token makeToken(TokenType type, std::size_t start, std::size_t line, std::size_t column) const;
        Token scanIdentifierOrKeyword();
        Token scanNumber();
        Token scanQuotedLiteral();
        Token scanCharCodeLiteral();
        Token scanBraceCommentOrDirective();
        Token scanParenStarCommentOrDirective();
        Token scanSlashComment();
        Token scanOperatorOrPunctuator();
        Token scanErrorToken();

        static std::string toLowerCopy(const std::string& value);
    };

    std::string formatToken(const Token& token);
    std::vector<std::string> formatTokens(const std::vector<Token>& tokens);

}  // namespace pascal_lexer
