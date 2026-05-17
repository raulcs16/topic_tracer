#pragma once

#include "icommand.hpp"
#include <map>


class CLIService {

public:
    CLIService();
    void registerCommand(std::unique_ptr<ICommand> cmd);
    bool hasCommand(const std::string &name) const;
    size_t commandSize() const { return m_commands.size(); }
    bool execute(std::string rawString);

private:
    std::map<std::string, std::unique_ptr<ICommand>> m_commands;
};