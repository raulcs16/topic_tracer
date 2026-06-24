#include "token.h"
#include "tokenizer.h"
#include <catch2/catch_test_macros.hpp>
#include <string>


TEST_CASE("Tokenizer::Words Words and More Words") {
    SECTION("No Input") {
        std::string input = "";
        auto tokens = Tokenizer::tokenize(input);
        REQUIRE(tokens.size() == 0);
    }
    SECTION(" Prefix") {
        std::string input = " Prefix";
        auto tokens = Tokenizer::tokenize(input);
        REQUIRE(tokens.size() == 1);
        CHECK(tokens.front().value == "Prefix");
    }
    SECTION(" Suffix") {
        std::string input = "Suffix ";
        auto tokens = Tokenizer::tokenize(input);
        REQUIRE(tokens.size() == 2);
        CHECK(tokens.front().value == "Suffix");
        CHECK(tokens.back().type == TokenType::Empty);
    }
    SECTION("Lots of Words") {
        std::string input = "hello world1! the brown fox jumped over the red fence";
        auto tokens = Tokenizer::tokenize(input);
        REQUIRE(tokens.size() > 1);
        for (const Token &t : tokens) {
            CHECK(t.type == TokenType::Word);
        }
    }
    SECTION("Skip = Skip") {
        std::string input = "Skip = Skip";
        auto tokens = Tokenizer::tokenize(input);
        REQUIRE(tokens.size() == 2);
        CHECK(tokens.front().value == "Skip");
        CHECK(tokens.back().value == "Skip");
    }
    SECTION("Double Quotes") {
        std::string input = "\"Skip  Skip\"";
        auto tokens = Tokenizer::tokenize(input);
        REQUIRE(tokens.size() == 1);
        CHECK(tokens.front().value == "Skip  Skip");
    }
    SECTION("Double = Quotes") {
        std::string input = "\"Skip = Skip\"";
        auto tokens = Tokenizer::tokenize(input);
        REQUIRE(tokens.size() == 1);
        CHECK(tokens.front().value == "Skip = Skip");
    }
    SECTION("Single Quotes") {
        std::string input = "\'Skip = Skip\'";
        auto tokens = Tokenizer::tokenize(input);
        REQUIRE(tokens.size() == 1);
        CHECK(tokens.front().value == "Skip = Skip");
    }
}
TEST_CASE("Tokenizer::Empty End") {
    std::string input = "Hello ";
    auto tokens = Tokenizer::tokenize(input);
    REQUIRE(tokens.size() == 2);
    CHECK(tokens[1].type == TokenType::Empty);
}
TEST_CASE("FLAGS") {
    std::string input = "git commit -m \"Hello World!\"";
    auto tokens = Tokenizer::tokenize(input);
    REQUIRE(tokens.size() == 4);
    CHECK(tokens[0].value == "git");
    CHECK(tokens[1].value == "commit");
    CHECK(tokens[2].value == "m");
    CHECK(tokens.back().value == "Hello World!");
}