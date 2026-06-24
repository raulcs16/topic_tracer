#pragma once

#include "icommand.hpp"
#include <map>


class RootCommand : public ICommand {
public:
    RootCommand() : ICommand("") {}

protected:
    bool doExecute(const std::map<std::string, std::string> &values) const override {
        return false;
    }
};
class CLIService {

public:
    CLIService();
    void registerCommand(std::unique_ptr<ICommand> cmd);
    size_t commandCount() { return m_cmd_count; }
    bool execute(const std::string &rawString);
    const ICommand *getCommand(const std::vector<Token> &tokens);

private:
    RootCommand m_root;
    size_t m_cmd_count = 0;
};