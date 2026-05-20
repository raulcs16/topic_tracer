#include "cli_service.hpp"
#include <catch2/catch_test_macros.hpp>


class MockCmd : public ICommand {

public:
    MockCmd(const std::string &name) : ICommand(name) {}

protected:
    bool doExecute(const std::map<std::string, std::string> &values) const override {
        return true;
    }
};


TEST_CASE("CLI:NestedCmds") {
    CLIService cli;
    auto node = std::make_unique<MockCmd>("node");
    auto edge = std::make_unique<MockCmd>("edge");


    SECTION("update cmd count") {
        cli.registerCommand(std::move(node));
        cli.registerCommand(std::move(edge));
        REQUIRE(cli.commandCount() == 2);
    }
    SECTION("no repeated counts") {
        cli.registerCommand(std::move(node));
        cli.registerCommand(std::move(node));
        REQUIRE(cli.commandCount() == 1);
    }
    SECTION("find nested cmd") {
        node->registerSubCommand(std::move(edge));
        cli.registerCommand(std::move(node));
        Token tnode{.type = TokenType::Word, .value = "node"};
        Token tedge{.type = TokenType::Word, .value = "edge"};
        auto cmd = cli.getCommand({tnode, tedge});
        REQUIRE(cmd != nullptr);
        CHECK(cmd->getCommandSignature().name() == "edge");
    }
};
