#pragma once

#include <string>

namespace pascal_lexer {

    class FileReader {
    public:
        static std::string readAllText(const std::string& path);
    };

}  // namespace pascal_lexer
