#pragma once

#include <string>

#include "ISourceReader.h"

namespace pascal_lexer {

    class FileReader final : public ISourceReader {
    public:
        std::string readAllText(const std::string& path) const override;
    };

}
