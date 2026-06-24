#include "command_signature.h"
#include "token.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("CommandSignature Validation and Mapping") {
    CommandSignature cmd("commit");
    SECTION("Recognize itslef") {
        std::vector<Token> tokens{{TokenType::Word, "commit", 0, 6}};
        MapResult result = cmd.mapping(tokens);

        REQUIRE(result.success == true);
        REQUIRE(result.error_message.empty());
        CHECK(result.values[TerminalKeys::Cmd] == "commit");
    }
    SECTION("Add Required Flag") {
        cmd.addShortFlag(Flag{.flag = "m",
                              .isBoolean = false,
                              .isRequired = true,
                              .providerKey = TerminalKeys::ValueFlag});
        std::vector<Token> tokens{
            {TokenType::Word, "commit", 0, 6},
            {TokenType::FlagGroup, "m", 7, 8},
            {TokenType::Word, "Hello World", 7, 8},
        };
        MapResult result = cmd.mapping(tokens);
        REQUIRE(result.success == true);
        CHECK(result.values["m"] == "Hello World");
    }
    SECTION("ADD Required Argument") {
        cmd.addPositionalArg(Argument{.name = "place", .providerKey = "locations"});
        std::vector<Token> tokens{
            {TokenType::Word, "commit", 0, 6},
            {TokenType::Word, "Baja California", 7, 15},
        };
        MapResult result = cmd.mapping(tokens);
        REQUIRE(result.success == true);
        CHECK(result.values["place"] == "Baja California");
    }
    SECTION("ADD OPTIONAL BOOLEAN FLAGS,forget the flag") {
        cmd.addShortFlag(Flag{.flag = "b",
                              .isBoolean = true,
                              .isRequired = false,
                              .providerKey = TerminalKeys::BoolFlags});
        std::vector<Token> tokens{
            {TokenType::Word, "commit", 0, 6},
        };
        MapResult result = cmd.mapping(tokens);
        REQUIRE(result.success == true);
    }
    SECTION("ADD OPTIONAL BOOLEAN FLAGS,add the flag") {
        cmd.addShortFlag(Flag{.flag = "b",
                              .isBoolean = true,
                              .isRequired = false,
                              .providerKey = TerminalKeys::BoolFlags});
        std::vector<Token> tokens{
            {TokenType::Word, "commit", 0, 6},
            {TokenType::FlagGroup, "b", 7, 8},
        };
        MapResult result = cmd.mapping(tokens);
        REQUIRE(result.success == true);
        CHECK(result.values["b"] == "true");
    }

    SECTION("ADD OPTIONAL BOOLEAN FLAGS,forget the flag") {
        cmd.addLongFlag(Flag{.flag = "b",
                             .isBoolean = true,
                             .isRequired = false,
                             .providerKey = TerminalKeys::BoolFlags});
        std::vector<Token> tokens{
            {TokenType::Word, "commit", 0, 6},
        };
        MapResult result = cmd.mapping(tokens);
        REQUIRE(result.success == true);
    }
    SECTION("ADD OPTIONAL BOOLEAN FLAGS,add the flag") {
        cmd.addLongFlag(Flag{.flag = "b",
                             .isBoolean = true,
                             .isRequired = false,
                             .providerKey = TerminalKeys::BoolFlags});
        std::vector<Token> tokens{
            {TokenType::Word, "commit", 0, 6},
            {TokenType::LongFlag, "b", 7, 8},
        };
        MapResult result = cmd.mapping(tokens);
        REQUIRE(result.success == true);
        CHECK(result.values["b"] == "true");
    }
    SECTION("Fails when required flag argument is missing") {

        cmd.addShortFlag(Flag{.flag = "m",
                              .isBoolean = false,
                              .isRequired = true,
                              .providerKey = TerminalKeys::ValueFlag});
        std::vector<Token> tokens = {
            {TokenType::Word, "commit", 0, 6},
        };

        MapResult result = cmd.mapping(tokens);

        // 1. Assert validation failed safely
        CHECK(result.success == false);
        CHECK(!result.error_message.empty()); // e.g., "Missing argument for flag -m"
        CHECK(result.error_token.value == "commit");
    }

    SECTION("Fails on completely unknown flags") {
        // Simulating tokens for: commit --verbose
        std::vector<Token> tokens = {{TokenType::Word, "commit", 0, 6},
                                     {TokenType::LongFlag, "--verbose", 7, 16}};

        MapResult result = cmd.mapping(tokens);

        CHECK(result.success == false);
        // It should flag "--verbose" as the illegal token
        CHECK(result.error_token.value == "--verbose");
    }
}