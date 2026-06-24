#include "default_keys.h"
#include "icommand.h"
#include <catch2/catch_test_macros.hpp>


class Commit : public ICommand {
public:
    Commit() : ICommand("commit") {
        m_signature.addShortFlag(Flag{.flag = "m",
                                      .isBoolean = false,
                                      .isRequired = true,
                                      .providerKey = TerminalKeys::None});
    }

protected:
    bool doExecute(const std::map<std::string, std::string> &values) const override {
        return values.contains("m");
    }
};
class Push : public ICommand {
public:
    Push() : ICommand("push") {
        m_signature.addShortFlag(Flag{.flag = "u",
                                      .isBoolean = true,
                                      .isRequired = false,
                                      .providerKey = TerminalKeys::BoolFlags});
    }

protected:
    bool doExecute(const std::map<std::string, std::string> &values) const override {
        return true;
    }
};

class Git : public ICommand {
public:
    Git() : ICommand("git") {
        registerSubCommand(std::make_unique<Commit>());
        registerSubCommand(std::make_unique<Push>());
    }

protected:
    bool doExecute(const std::map<std::string, std::string> &values) const override {
        return true;
    }
};
TEST_CASE("ICommand::Git") {
    Git command;
    SECTION("Commit") {
        auto tokens = Tokenizer::tokenize("git commit -m \"Hello World\"");
        auto result = command.execute(tokens);
        REQUIRE(result == true);
    }
    SECTION("PUSH") {
        auto tokens = Tokenizer::tokenize("git push");
        auto result = command.execute(tokens);
        REQUIRE(result == true);
    }
    SECTION("PUSH -u") {
        auto tokens = Tokenizer::tokenize("git push -u");
        auto result = command.execute(tokens);
        REQUIRE(result == true);
    }
}
