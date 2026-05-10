#pragma once

#include <string>

#include <gmock/gmock.h>

#include "ISourceReader.h"

namespace pascal_lexer_tests {

    class MockSourceReader : public pascal_lexer::ISourceReader {
    public:
        MOCK_METHOD(std::string, readAllText,
            (const std::string& path), (const, override));
    };

}