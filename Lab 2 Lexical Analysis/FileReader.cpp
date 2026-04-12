#include "FileReader.h"

#include <fstream>
#include <iterator>
#include <stdexcept>

namespace pascal_lexer {

    std::string FileReader::readAllText(const std::string& path) {
        std::ifstream input(path, std::ios::binary);
        if (!input.is_open()) {
            throw std::runtime_error("Could not open source file: " + path);
        }

        return std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
    }

}
