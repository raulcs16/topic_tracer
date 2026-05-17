#include "command_signature.hpp"
#include "default_keys.hpp"
#include <catch2/catch_test_macros.hpp>


TEST_CASE("Keys: Basic & Flag Grouping") {
    CommandSignature node("node");
    Token cmd{.value = "node", .type = TokenType::Word};

    SECTION("correctly return cmd key") {
        auto keys = node.keys({cmd});
        REQUIRE(keys.size() == 1);
        CHECK(keys[0] == TerminalKeys::Cmd);
    }
    node.addPositionalArg(Argument{.providerKey = "graph.nodes.name", .name = "src"});
    Token empty{.value = "", .type = TokenType::Empty};
    SECTION("correctly return next positional arg provider") {
        auto keys = node.keys({cmd, empty});
        REQUIRE(keys.size() == 2);
        CHECK(keys.back() == "graph.nodes.name");
    }
    node.addShortFlag(
        Flag{.flag = "t", .providerKey = "graph.node.types", .isBoolean = false});
    Token t{.value = "t", .type = TokenType::FlagGroup};
    SECTION("given a value flag return correct arg") {
        //"node -t "
        auto keys = node.keys({cmd, t, empty});
        REQUIRE(keys.size() == 3);
        CHECK(keys.back() == "graph.node.types");
    }
    SECTION("given a value flag return correct arg") {
        //"node -t"
        auto keys = node.keys({cmd, t});
        REQUIRE(keys.size() == 3);
        CHECK(keys.back() == "graph.node.types");
    }
    node.addShortFlag(Flag{.flag = "v", .isBoolean = true});
    Token v{.value = "v", .type = TokenType::FlagGroup};
    //"node -v"
    SECTION("given bool flag return boolflag at end") {
        auto keys = node.keys({cmd, v});
        REQUIRE(keys.size() == 2);
        CHECK(keys.back() == TerminalKeys::BoolFlags);
    }
    node.addShortFlag(Flag{.flag = "a", .isBoolean = true});
    Token group{.value = "av", .type = TokenType::FlagGroup};
    SECTION("Group of bool flags") {
        auto keys = node.keys({cmd, group});
        REQUIRE(keys.size() == 2);
        CHECK(keys.back() == TerminalKeys::BoolFlags);
    }
    SECTION("given bool flag + empty space return next arg provider") {
        //"node -v "
        auto keys = node.keys({cmd, v, empty});
        REQUIRE(keys.size() == 3);
        CHECK(keys.back() == "graph.nodes.name");
    }
    node.addLongFlag(Flag{.flag = "verbose", .isBoolean = true});
    Token verbose{.value = "verbose", .type = TokenType::LongFlag};
    SECTION("handle boolean long flag") {
        auto keys = node.keys({cmd, verbose, empty});
        REQUIRE(keys.size() == 3);
        CHECK(keys.back() == "graph.nodes.name");
    }
    node.addLongFlag(
        Flag{.flag = "type", .providerKey = "graph.node.types", .isBoolean = false});
    Token type{.value = "type", .type = TokenType::LongFlag};
    SECTION("handle value long flag") {
        auto keys = node.keys({cmd, type, empty});
        REQUIRE(keys.size() == 3);
        CHECK(keys.back() == "graph.node.types");
    }
}

TEST_CASE("Validate Commands") {
    //add a -t Person
    CommandSignature addNode("add");
    addNode.addPositionalArg(Argument{.name = "node_name"});
    addNode.addShortFlag(Flag{.flag = "t",
                              .isBoolean = false,
                              .isRequired = true,
                              .providerKey = "graph.nodes.types"});
    Token add{.value = "add", .type = TokenType::Word};
    Token sub{.value = "sub", .type = TokenType::Word};
    Token node_name{.value = "john", .type = TokenType::Word};
    Token flag{.value = "t", .type = TokenType::FlagGroup};
    Token node_type{.value = "Person", .type = TokenType::Word};

    Token falseFlag{.value = "false", .type = TokenType::LongFlag};
    Token mixedFlag{.value = "tf", .type = TokenType::FlagGroup};

    SECTION("Not Enough Arguements") {
        auto result = addNode.mapping({add, node_name});
        REQUIRE(result.success == false);
    }
    SECTION("Enough Arguements, but wrong cmd") {
        auto result = addNode.mapping({sub, node_name, flag, node_type});
        REQUIRE(result.success == false);
    }
    SECTION("Enough Arguements,prefix word,correct cmd ") {
        auto result = addNode.mapping({node_name, add, flag, node_type});
        REQUIRE(result.success == false);
    }
    SECTION("Enough Arguements,correct cmd, but not enough positional args") {
        auto result = addNode.mapping({add, flag, node_type, flag});
        REQUIRE(result.success == false);
    }
    SECTION("valid command option") {
        auto result = addNode.mapping({add, flag, node_type, node_name});
        REQUIRE(result.success);
    }
    SECTION("valid command option") {
        auto result = addNode.mapping({add, node_name, flag, node_type});
        REQUIRE(result.success);
    }
    SECTION("extra false flag") {
        auto result = addNode.mapping({add, node_name, flag, node_type, falseFlag});
        REQUIRE(result.success == false);
    }
    SECTION("mixed group flag") {
        auto result = addNode.mapping({add, node_name, mixedFlag, node_type});
        REQUIRE(result.success == false);
    }
}