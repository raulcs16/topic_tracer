#include "tokenizer.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Correctly Return Token values with start and end positions") {

    SECTION("Empty") {
        auto tokens = Tokenizer::tokenize("");
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0].type == TokenType::Empty);
        REQUIRE(tokens[0].value == "");
        REQUIRE(tokens[0].start == 0);
        REQUIRE(tokens[0].end == 0);
    }
    SECTION("Gap") {
        std::string gap = "    ";
        auto tokens = Tokenizer::tokenize(gap);
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0].type == TokenType::Empty);
        REQUIRE(tokens[0].value == "");
        REQUIRE(tokens[0].start == gap.size());
        REQUIRE(tokens[0].end == gap.size());
    }

    SECTION("basic positional words") {
        auto tokens = Tokenizer::tokenize("one two");

        REQUIRE(tokens.size() == 2);
        CHECK(tokens[0].value == "one");
        CHECK(tokens[0].type == TokenType::Word);
        CHECK(tokens[0].start == 0);
        CHECK(tokens[0].end == 2); // 'n' to '1' is 5 chars, index 0-4

        CHECK(tokens[1].value == "two");
        CHECK(tokens[1].type == TokenType::Word);
        CHECK(tokens[1].start == 4);
        CHECK(tokens[1].end == 6);
    }
    SECTION("White Space prefixed") {
        auto tokens = Tokenizer::tokenize(" one two");

        REQUIRE(tokens.size() == 2);
        CHECK(tokens[0].value == "one");
        CHECK(tokens[0].type == TokenType::Word);
        CHECK(tokens[0].start == 1);
        CHECK(tokens[0].end == 3); // 'n' to '1' is 5 chars, index 0-4

        CHECK(tokens[1].value == "two");
        CHECK(tokens[1].type == TokenType::Word);
        CHECK(tokens[1].start == 5);
        CHECK(tokens[1].end == 7);
    }
}
TEST_CASE("Trailing Space") {

    SECTION("ls ") {
        auto tokens = Tokenizer::tokenize("ls ");

        REQUIRE(tokens.size() == 2);
        CHECK(tokens[0].value == "ls");
        CHECK(tokens[1].type == TokenType::Empty);
        CHECK(tokens[1].start == 3); // Cursor is at the end of the string
    }
    SECTION("ls     ") {
        auto tokens = Tokenizer::tokenize("ls     ");

        // Still only 2 tokens! We skip the middle "void"
        REQUIRE(tokens.size() == 2);
        CHECK(tokens[0].value == "ls");
        CHECK(tokens[1].type == TokenType::Empty);
        CHECK(tokens[1].start == 7); // Total length of string is 7
    }
}
TEST_CASE("Correctly identify Flags") {
    SECTION("flag indicated but no flag value") {
        auto tokens = Tokenizer::tokenize("-");
        REQUIRE(tokens.size() == 1);
        CHECK(tokens[0].type == TokenType::FlagGroup);
        CHECK(tokens[0].start == 0);
        CHECK(tokens[0].end == 0);
        CHECK(tokens[0].value == "");
    }
    SECTION("single flag passed in") {
        auto tokens = Tokenizer::tokenize("-t");
        REQUIRE(tokens.size() == 1);
        CHECK(tokens[0].type == TokenType::FlagGroup);
        CHECK(tokens[0].start == 0);
        CHECK(tokens[0].end == 1);
        CHECK(tokens[0].value == "t");
    }
    SECTION("parse flagroup passed in") {
        auto tokens = Tokenizer::tokenize("-xyz");
        REQUIRE(tokens.size() == 1);
        CHECK(tokens[0].type == TokenType::FlagGroup);
        CHECK(tokens[0].start == 0);
        CHECK(tokens[0].end == 3);
        CHECK(tokens[0].value == "xyz");
    }
    SECTION("parse longflag") {
        std::string flag = "--verbose";
        auto tokens = Tokenizer::tokenize(flag);
        REQUIRE(tokens.size() == 1);
        CHECK(tokens[0].type == TokenType::LongFlag);
        CHECK(tokens[0].start == 0);
        CHECK(tokens[0].end == flag.size() - 1);
        CHECK(tokens[0].value == "verbose");
    }
    SECTION("MULTIPE FlagsGroups") {
        std::string flag_a = "-f";
        std::string flag_b = "-t";
        std::string verbose = "--verbose";
        std::string flags = flag_a + " " + flag_b + " " + verbose;
        auto tokens = Tokenizer::tokenize(flags);
        REQUIRE(tokens.size() == 3);
        CHECK(tokens[0].type == TokenType::FlagGroup);
        CHECK(tokens[0].start == 0);
        CHECK(tokens[0].end == flag_a.size() - 1);
        CHECK(tokens[0].value == "f");

        CHECK(tokens[1].type == TokenType::FlagGroup);
        CHECK(tokens[1].start == flag_a.size() + 1);
        CHECK(tokens[1].end == flag_a.size() + flag_b.size());
        CHECK(tokens[1].value == "t");

        CHECK(tokens[2].type == TokenType::LongFlag);
        CHECK(tokens[2].start == flag_a.size() + 1 + flag_b.size() + 1);
        CHECK(tokens[2].end == flags.size() - 1);
        CHECK(tokens[2].value == "verbose");
    }
}
TEST_CASE("Words and Flags") {
    SECTION("ls -a") {
        auto tokens = Tokenizer::tokenize("ls -a");
        REQUIRE(tokens.size() == 2);
        CHECK(tokens[0].type == TokenType::Word);
        CHECK(tokens[1].type == TokenType::FlagGroup);
    }
    SECTION("ls -a file") {
        auto tokens = Tokenizer::tokenize("ls -a file");
        REQUIRE(tokens.size() == 3);
        CHECK(tokens[0].type == TokenType::Word);
        CHECK(tokens[1].type == TokenType::FlagGroup);
        CHECK(tokens[2].type == TokenType::Word);
    }
    SECTION("ls -al file") {
        auto tokens = Tokenizer::tokenize("ls -a file");
        REQUIRE(tokens.size() == 3);
        CHECK(tokens[0].type == TokenType::Word);
        CHECK(tokens[1].type == TokenType::FlagGroup);
        CHECK(tokens[2].type == TokenType::Word);
    }
    SECTION("ls -al file --verbose") {
        auto tokens = Tokenizer::tokenize("ls -a file --verbose");
        REQUIRE(tokens.size() == 4);
        CHECK(tokens[0].type == TokenType::Word);
        CHECK(tokens[1].type == TokenType::FlagGroup);
        CHECK(tokens[2].type == TokenType::Word);
        CHECK(tokens[3].type == TokenType::LongFlag);
    }
}
TEST_CASE("Dealing with Value Flags") {
    SECTION("flag word") {
        auto tokens = Tokenizer::tokenize("ls -t sometype");
        REQUIRE(tokens.size() == 3);
        CHECK(tokens[0].type == TokenType::Word);
        CHECK(tokens[1].type == TokenType::FlagGroup);
        CHECK(tokens[2].type == TokenType::Word);
    }
    SECTION("flag=value") {
        auto tokens = Tokenizer::tokenize("ls -t=sometype");
        REQUIRE(tokens.size() == 3);
        CHECK(tokens[0].type == TokenType::Word);
        CHECK(tokens[1].type == TokenType::FlagGroup);
        CHECK(tokens[2].type == TokenType::Word);
    }
    SECTION("flag= value") {
        auto tokens = Tokenizer::tokenize("ls -t= sometype");
        REQUIRE(tokens.size() == 3);
        CHECK(tokens[0].type == TokenType::Word);
        CHECK(tokens[1].type == TokenType::FlagGroup);
        CHECK(tokens[2].type == TokenType::Word);
    }
    SECTION("flag =value") {
        auto tokens = Tokenizer::tokenize("ls -t =sometype");
        REQUIRE(tokens.size() == 3);
        CHECK(tokens[0].type == TokenType::Word);
        CHECK(tokens[1].type == TokenType::FlagGroup);
        CHECK(tokens[2].type == TokenType::Word);
    }
    SECTION("flag = value") {
        auto tokens = Tokenizer::tokenize("ls -t = sometype");
        REQUIRE(tokens.size() == 3);
        CHECK(tokens[0].type == TokenType::Word);
        CHECK(tokens[1].type == TokenType::FlagGroup);
        CHECK(tokens[2].type == TokenType::Word);
    }
}
