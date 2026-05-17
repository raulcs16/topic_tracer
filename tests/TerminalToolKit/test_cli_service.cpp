#include "cli_service.hpp"
#include <catch2/catch_test_macros.hpp>

class TestCmd : public ICommand {
public:
    TestCmd() : ICommand{"test"} {}

protected:
    bool doExecute(const std::map<std::string, std::string> &) override { return true; }
};
class MestCmd : public ICommand {
public:
    MestCmd() : ICommand{"mest"} {}

protected:
    bool doExecute(const std::map<std::string, std::string> &) override { return true; }
};

class LinkCmd : public ICommand {
public:
    LinkCmd() : ICommand{"link"} {
        m_signature.addShortFlag(

            Flag{.flag = "t", .isBoolean = false, .providerKey = "graph.edge.type"});
        m_signature.addPositionalArg(
            Argument{.name = "src", .providerKey = "graph.node.name"});
        m_signature.addPositionalArg(
            Argument{.name = "target", .providerKey = "graph.node.name"});
    }

protected:
    bool doExecute(const std::map<std::string, std::string> &) override { return true; }
};
CLIService cli;
TEST_CASE("CLI::UnderTest") {
    cli.registerCommand(std::make_unique<TestCmd>());
    cli.registerCommand(std::make_unique<MestCmd>());
    cli.registerCommand(std::make_unique<LinkCmd>());
    SECTION("cli:register") {
        REQUIRE(cli.commandSize() == 3);
        CHECK(cli.hasCommand("test"));
        CHECK(cli.hasCommand("mest"));
        CHECK(cli.hasCommand("link"));
    }
}
class RmNode : public ICommand {
public:
    RmNode() : ICommand{"rm"} {
        m_signature.addPositionalArg(
            Argument{.name = "node_name", .providerKey = "graph.nodes.names"});
    }

protected:
    bool doExecute(const std::map<std::string, std::string> &val) override {
        return true;
    }
};
class AddNode : public ICommand {
public:
    AddNode() : ICommand{"add"} {
        m_signature.addShortFlag(Flag{.flag = "t",
                                      .isBoolean = false,
                                      .isRequired = true,
                                      .providerKey = "graph.node.types"});
        m_signature.addPositionalArg(Argument{.name = "node_name"});
    }

protected:
    bool doExecute(const std::map<std::string, std::string> &val) override {
        return true;
    }
};

class NodeNameSpace : public ICommand {
public:
    NodeNameSpace() : ICommand{"node"} {}

protected:
    bool doExecute(const std::map<std::string, std::string> &val) override {
        return true;
    }
};
TEST_CASE("CLI::Nested Comands") {
    auto rootNode = std::make_unique<NodeNameSpace>();
    auto addCmd = std::make_unique<AddNode>();
    auto rmCmd = std::make_unique<RmNode>();

    rootNode->registerSubCommand(std::move(addCmd));
    rootNode->registerSubCommand(std::move(rmCmd));

    CLIService cli;
    cli.registerCommand(std::move(rootNode));
    REQUIRE(cli.execute("node add node_a -t person") == true);
}
