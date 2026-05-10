#include <sstream>
#include <stdexcept>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "LexerApplication.h"
#include "MockSourceReader.h"

using namespace testing;

namespace {

    using pascal_lexer::LexerApplication;
    using pascal_lexer_tests::MockSourceReader;

    TEST(LexerApplicationMockTests, ReadsSourceViaMockAndPrintsTokens) {
        StrictMock<MockSourceReader> reader;
        LexerApplication app(reader);

        std::ostringstream out;
        std::ostringstream err;

        EXPECT_CALL(reader, readAllText(AllOf(HasSubstr("demo"), HasSubstr(".pas"))))
            .Times(1)
            .WillOnce(Return("begin x := 42;"));

        const int exitCode = app.run("demo_program.pas", out, err);

        EXPECT_EQ(exitCode, 0);
        EXPECT_TRUE(err.str().empty());

        EXPECT_THAT(out.str(), HasSubstr("<begin, KEYWORD>"));
        EXPECT_THAT(out.str(), HasSubstr("<:=, OPERATOR>"));
        EXPECT_THAT(out.str(), HasSubstr("<;, PUNCTUATOR>"));
    }

    TEST(LexerApplicationMockTests, ReturnsErrorCodeAndWritesMessageWhenReaderThrows) {
        StrictMock<MockSourceReader> reader;
        LexerApplication app(reader);

        std::ostringstream out;
        std::ostringstream err;

        EXPECT_CALL(reader, readAllText(Eq("restricted.pas")))
            .Times(1)
            .WillOnce(Throw(std::runtime_error("Access denied")));

        const int exitCode = app.run("restricted.pas", out, err);

        EXPECT_EQ(exitCode, 1);
        EXPECT_TRUE(out.str().empty());
        EXPECT_THAT(err.str(), HasSubstr("Access denied"));
    }

    TEST(LexerApplicationMockTests, CallsReaderInStrictOrderForDifferentRuns) {
        StrictMock<MockSourceReader> reader;
        LexerApplication app(reader);

        std::ostringstream out1;
        std::ostringstream err1;
        std::ostringstream out2;
        std::ostringstream err2;

        {
            InSequence sequence;

            EXPECT_CALL(reader, readAllText(Eq("first.pas")))
                .Times(1)
                .WillOnce(Return("begin first := 1;"));

            EXPECT_CALL(reader, readAllText(Eq("second.pas")))
                .Times(1)
                .WillOnce(Return("begin second := 2;"));
        }

        EXPECT_EQ(app.run("first.pas", out1, err1), 0);
        EXPECT_EQ(app.run("second.pas", out2, err2), 0);

        EXPECT_THAT(out1.str(), HasSubstr("first"));
        EXPECT_THAT(out2.str(), HasSubstr("second"));
        EXPECT_TRUE(err1.str().empty());
        EXPECT_TRUE(err2.str().empty());
    }

    TEST(LexerApplicationMockTests, CanReturnDifferentSourceTextOnSequentialCalls) {
        StrictMock<MockSourceReader> reader;
        LexerApplication app(reader);

        std::ostringstream out1;
        std::ostringstream err1;
        std::ostringstream out2;
        std::ostringstream err2;

        EXPECT_CALL(reader, readAllText(Eq("same_input.pas")))
            .Times(2)
            .WillOnce(Return("begin alpha := 1;"))
            .WillOnce(Return("begin beta := 2;"));

        EXPECT_EQ(app.run("same_input.pas", out1, err1), 0);
        EXPECT_EQ(app.run("same_input.pas", out2, err2), 0);

        EXPECT_THAT(out1.str(), HasSubstr("alpha"));
        EXPECT_THAT(out2.str(), HasSubstr("beta"));
        EXPECT_TRUE(err1.str().empty());
        EXPECT_TRUE(err2.str().empty());
    }

}