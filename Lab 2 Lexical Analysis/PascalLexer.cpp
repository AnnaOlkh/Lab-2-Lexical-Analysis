#include "PascalLexer.h"

#include <algorithm>
#include <cctype>

namespace pascal_lexer {

    namespace {

        bool isIdentifierStart(char value) {
            return std::isalpha(static_cast<unsigned char>(value)) != 0 || value == '_';
        }

        bool isIdentifierPart(char value) {
            return std::isalnum(static_cast<unsigned char>(value)) != 0 || value == '_';
        }

        bool isDigit(char value) {
            return std::isdigit(static_cast<unsigned char>(value)) != 0;
        }

        bool isHexDigit(char value) {
            return std::isxdigit(static_cast<unsigned char>(value)) != 0;
        }

    }

    PascalLexer::PascalLexer(std::string source)
        : source_(std::move(source)), position_(0), line_(1), column_(1) {
    }

    const std::unordered_set<std::string>& PascalLexer::keywords() {
        static const std::unordered_set<std::string> values = {
            "and", "array", "asm", "begin", "case", "const", "constructor", "destructor",
            "dispinterface", "div", "do", "downto", "else", "end", "except", "exports",
            "file", "finalization", "finally", "for", "function", "goto", "if", "implementation",
            "inherited", "initialization", "inline", "interface", "label", "library", "mod", "nil",
            "not", "object", "of", "or", "packed", "procedure", "program", "property", "raise",
            "record", "repeat", "resourcestring", "set", "shl", "shr", "string", "then", "threadvar",
            "to", "try", "type", "unit", "until", "uses", "var", "while", "with", "xor",
            "class", "private", "protected", "public", "published", "boolean", "byte", "char",
            "double", "extended", "integer", "int64", "longint", "real", "shortint", "smallint",
            "word", "write", "writeln", "read", "readln", "true", "false"
        };
        return values;
    }

    const std::unordered_set<std::string>& PascalLexer::wordOperators() {
        static const std::unordered_set<std::string> values = {
            "and", "div", "in", "mod", "not", "or", "shl", "shr", "xor", "as", "is"
        };
        return values;
    }

    bool PascalLexer::isAtEnd() const {
        return position_ >= source_.size();
    }

    char PascalLexer::peek(std::size_t offset) const {
        const std::size_t index = position_ + offset;
        if (index >= source_.size()) {
            return '\0';
        }
        return source_[index];
    }

    char PascalLexer::advance() {
        const char current = source_[position_++];
        if (current == '\n') {
            ++line_;
            column_ = 1;
        }
        else {
            ++column_;
        }
        return current;
    }

    bool PascalLexer::match(char expected) {
        if (isAtEnd() || peek() != expected) {
            return false;
        }
        advance();
        return true;
    }

    void PascalLexer::skipWhitespace() {
        while (!isAtEnd() && std::isspace(static_cast<unsigned char>(peek())) != 0) {
            advance();
        }
    }

    Token PascalLexer::makeToken(TokenType type, std::size_t start, std::size_t line, std::size_t column) const {
        return Token(type, source_.substr(start, position_ - start), line, column);
    }

    std::string PascalLexer::toLowerCopy(const std::string& value) {
        std::string result = value;
        std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
            });
        return result;
    }

    Token PascalLexer::scanIdentifierOrKeyword() {
        const std::size_t start = position_;
        const std::size_t tokenLine = line_;
        const std::size_t tokenColumn = column_;

        advance();
        while (isIdentifierPart(peek())) {
            advance();
        }

        const std::string lexeme = source_.substr(start, position_ - start);
        const std::string normalized = toLowerCopy(lexeme);

        if (wordOperators().count(normalized) != 0U) {
            return Token(TokenType::Operator, lexeme, tokenLine, tokenColumn);
        }
        if (keywords().count(normalized) != 0U) {
            return Token(TokenType::Keyword, lexeme, tokenLine, tokenColumn);
        }
        return Token(TokenType::Identifier, lexeme, tokenLine, tokenColumn);
    }

    Token PascalLexer::scanNumber() {
        const std::size_t start = position_;
        const std::size_t tokenLine = line_;
        const std::size_t tokenColumn = column_;

        if (peek() == '$') {
            advance();
            while (isHexDigit(peek())) {
                advance();
            }

            if (position_ == start + 1) {
                return Token(TokenType::Error, source_.substr(start, position_ - start), tokenLine, tokenColumn);
            }
            return makeToken(TokenType::HexLiteral, start, tokenLine, tokenColumn);
        }

        while (isDigit(peek())) {
            advance();
        }

        bool isFloat = false;
        if (peek() == '.' && peek(1) != '.') {
            isFloat = true;
            advance();
            while (isDigit(peek())) {
                advance();
            }
        }

        if (peek() == 'e' || peek() == 'E') {
            const std::size_t exponentPosition = position_;
            const std::size_t exponentColumn = column_;

            advance();
            if (peek() == '+' || peek() == '-') {
                advance();
            }

            if (!isDigit(peek())) {
                position_ = exponentPosition;
                column_ = exponentColumn;
            }
            else {
                isFloat = true;
                while (isDigit(peek())) {
                    advance();
                }
            }
        }

        return makeToken(isFloat ? TokenType::FloatLiteral : TokenType::IntegerLiteral, start, tokenLine, tokenColumn);
    }

    Token PascalLexer::scanQuotedLiteral() {
        const std::size_t start = position_;
        const std::size_t tokenLine = line_;
        const std::size_t tokenColumn = column_;

        advance();
        bool terminated = false;
        while (!isAtEnd()) {
            if (peek() == '\'') {
                advance();
                if (peek() == '\'') {
                    advance();
                    continue;
                }
                terminated = true;
                break;
            }
            advance();
        }

        if (!terminated) {
            return makeToken(TokenType::Error, start, tokenLine, tokenColumn);
        }

        const std::string lexeme = source_.substr(start, position_ - start);
        std::size_t contentLength = 0;
        for (std::size_t index = 1; index + 1 < lexeme.size(); ++index) {
            if (lexeme[index] == '\'' && index + 1 < lexeme.size() - 1 && lexeme[index + 1] == '\'') {
                ++contentLength;
                ++index;
            }
            else {
                ++contentLength;
            }
        }

        return Token(contentLength == 1 ? TokenType::CharLiteral : TokenType::StringLiteral, lexeme, tokenLine, tokenColumn);
    }

    Token PascalLexer::scanCharCodeLiteral() {
        const std::size_t start = position_;
        const std::size_t tokenLine = line_;
        const std::size_t tokenColumn = column_;

        advance();
        while (isDigit(peek())) {
            advance();
        }

        if (position_ == start + 1) {
            return makeToken(TokenType::Error, start, tokenLine, tokenColumn);
        }

        return makeToken(TokenType::CharLiteral, start, tokenLine, tokenColumn);
    }

    Token PascalLexer::scanBraceCommentOrDirective() {
        const std::size_t start = position_;
        const std::size_t tokenLine = line_;
        const std::size_t tokenColumn = column_;
        const bool isDirective = peek(1) == '$';

        advance();
        while (!isAtEnd() && peek() != '}') {
            advance();
        }

        if (isAtEnd()) {
            return makeToken(TokenType::Error, start, tokenLine, tokenColumn);
        }

        advance();
        return makeToken(isDirective ? TokenType::PreprocessorDirective : TokenType::Comment, start, tokenLine, tokenColumn);
    }

    Token PascalLexer::scanParenStarCommentOrDirective() {
        const std::size_t start = position_;
        const std::size_t tokenLine = line_;
        const std::size_t tokenColumn = column_;
        const bool isDirective = peek(2) == '$';

        advance();
        advance();

        while (!isAtEnd()) {
            if (peek() == '*' && peek(1) == ')') {
                advance();
                advance();
                return makeToken(isDirective ? TokenType::PreprocessorDirective : TokenType::Comment, start, tokenLine, tokenColumn);
            }
            advance();
        }

        return makeToken(TokenType::Error, start, tokenLine, tokenColumn);
    }

    Token PascalLexer::scanSlashComment() {
        const std::size_t start = position_;
        const std::size_t tokenLine = line_;
        const std::size_t tokenColumn = column_;

        advance();
        advance();
        while (!isAtEnd() && peek() != '\n') {
            advance();
        }

        return makeToken(TokenType::Comment, start, tokenLine, tokenColumn);
    }

    Token PascalLexer::scanOperatorOrPunctuator() {
        const std::size_t start = position_;
        const std::size_t tokenLine = line_;
        const std::size_t tokenColumn = column_;
        const char current = peek();

        switch (current) {
        case ':':
            advance();
            if (match('=')) {
                return makeToken(TokenType::Operator, start, tokenLine, tokenColumn);
            }
            return makeToken(TokenType::Punctuator, start, tokenLine, tokenColumn);
        case '<':
            advance();
            if (match('=') || match('>')) {
                return makeToken(TokenType::Operator, start, tokenLine, tokenColumn);
            }
            return makeToken(TokenType::Operator, start, tokenLine, tokenColumn);
        case '>':
            advance();
            match('=');
            return makeToken(TokenType::Operator, start, tokenLine, tokenColumn);
        case '.':
            advance();
            if (match('.')) {
                return makeToken(TokenType::Operator, start, tokenLine, tokenColumn);
            }
            return makeToken(TokenType::Punctuator, start, tokenLine, tokenColumn);
        case '+':
        case '-':
        case '*':
        case '/':
        case '=':
        case '^':
        case '@':
            advance();
            return makeToken(TokenType::Operator, start, tokenLine, tokenColumn);
        case ';':
        case ',':
        case '(':
        case ')':
        case '[':
        case ']':
            advance();
            return makeToken(TokenType::Punctuator, start, tokenLine, tokenColumn);
        default:
            return scanErrorToken();
        }
    }

    Token PascalLexer::scanErrorToken() {
        const std::size_t start = position_;
        const std::size_t tokenLine = line_;
        const std::size_t tokenColumn = column_;
        advance();
        return makeToken(TokenType::Error, start, tokenLine, tokenColumn);
    }

    std::vector<Token> PascalLexer::tokenize() {
        std::vector<Token> tokens;
        while (!isAtEnd()) {
            skipWhitespace();
            if (isAtEnd()) {
                break;
            }

            if (isIdentifierStart(peek())) {
                tokens.push_back(scanIdentifierOrKeyword());
                continue;
            }
            if (peek() == '$' || isDigit(peek())) {
                tokens.push_back(scanNumber());
                continue;
            }
            if (peek() == '\'') {
                tokens.push_back(scanQuotedLiteral());
                continue;
            }
            if (peek() == '#') {
                tokens.push_back(scanCharCodeLiteral());
                continue;
            }
            if (peek() == '{') {
                tokens.push_back(scanBraceCommentOrDirective());
                continue;
            }
            if (peek() == '(' && peek(1) == '*') {
                tokens.push_back(scanParenStarCommentOrDirective());
                continue;
            }
            if (peek() == '/' && peek(1) == '/') {
                tokens.push_back(scanSlashComment());
                continue;
            }

            tokens.push_back(scanOperatorOrPunctuator());
        }

        tokens.emplace_back(TokenType::EndOfFile, "", line_, column_);
        return tokens;
    }

    std::string formatToken(const Token& token) {
        return "<" + token.lexeme + ", " + tokenTypeToString(token.type) + ">";
    }

    std::vector<std::string> formatTokens(const std::vector<Token>& tokens) {
        std::vector<std::string> lines;
        lines.reserve(tokens.size());
        for (const Token& token : tokens) {
            lines.push_back(formatToken(token));
        }
        return lines;
    }

}
