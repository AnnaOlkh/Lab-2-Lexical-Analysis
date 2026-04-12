#pragma once

#include <string>

namespace pascal_lexer {

    enum class TokenType {
        Keyword,
        Operator,
        Identifier,
        IntegerLiteral,
        FloatLiteral,
        HexLiteral,
        StringLiteral,
        CharLiteral,
        PreprocessorDirective,
        Comment,
        Punctuator,
        Error,
        EndOfFile
    };

    struct Token {
        TokenType type;
        std::string lexeme;
        std::size_t line;
        std::size_t column;

        Token(TokenType tokenType, std::string tokenLexeme, std::size_t tokenLine, std::size_t tokenColumn)
            : type(tokenType), lexeme(std::move(tokenLexeme)), line(tokenLine), column(tokenColumn) {
        }
    };

    inline std::string tokenTypeToString(TokenType type) {
        switch (type) {
        case TokenType::Keyword: return "KEYWORD";
        case TokenType::Operator: return "OPERATOR";
        case TokenType::Identifier: return "IDENTIFIER";
        case TokenType::IntegerLiteral: return "INTEGER_LITERAL";
        case TokenType::FloatLiteral: return "FLOAT_LITERAL";
        case TokenType::HexLiteral: return "HEX_LITERAL";
        case TokenType::StringLiteral: return "STRING_LITERAL";
        case TokenType::CharLiteral: return "CHAR_LITERAL";
        case TokenType::PreprocessorDirective: return "PREPROCESSOR_DIRECTIVE";
        case TokenType::Comment: return "COMMENT";
        case TokenType::Punctuator: return "PUNCTUATOR";
        case TokenType::Error: return "ERROR";
        case TokenType::EndOfFile: return "EOF";
        }
        return "UNKNOWN";
    }

    inline bool operator==(const Token& left, const Token& right) {
        return left.type == right.type &&
            left.lexeme == right.lexeme &&
            left.line == right.line &&
            left.column == right.column;
    }

}
