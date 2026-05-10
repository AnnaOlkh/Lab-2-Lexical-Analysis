#pragma once

#include <iosfwd>
#include <string>

#include "ISourceReader.h"

namespace pascal_lexer {

    class LexerApplication {
    public:
        explicit LexerApplication(const ISourceReader& sourceReader);

        int run(
            const std::string& inputPath,
            std::ostream& output,
            std::ostream& error
        ) const;

    private:
        const ISourceReader& sourceReader_;
    };

}