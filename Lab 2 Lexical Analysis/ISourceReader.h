#pragma once

#include <string>

namespace pascal_lexer {

    class ISourceReader {
    public:
        virtual ~ISourceReader() = default;

        virtual std::string readAllText(const std::string& path) const = 0;
    };

}