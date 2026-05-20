#include "icommand.hpp"
#include <catch2/catch_test_macros.hpp>

class MockCommand : public ICommand {

public:
    MockCommand(const std::string &name) : ICommand(name) {}

protected:
    bool doExecute(const std::map<std::string, std::string> &values) const override {
        return true;
    }
};

TEST_CASE("Nested: single nest") {
    MockCommand root("");
    auto node = std::make_unique<MockCommand>("node");
    auto edge = std::make_unique<MockCommand>("edge");
    root.registerSubCommand(std::move(node));
    root.registerSubCommand(std::move(edge));

    SECTION("return node") {
        Token tnode{.type = TokenType::Word, .value = "node"};
        auto sub = root.getSubCommand({tnode});
        auto name = sub->getCommandSignature().name();
        REQUIRE(sub != nullptr);
        CHECK(name == "node");
    }
}