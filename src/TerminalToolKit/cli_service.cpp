#include "cli_service.hpp"
#include "tokenizer.hpp"


CLIService::CLIService() {}
void CLIService::registerCommand(std::unique_ptr<ICommand> cmd) {
    auto def = cmd->getCommandSignature();
    auto name = def.name();
    auto it = m_commands.find(name);
    if (it != m_commands.end()) //found
        return;

    m_commands[name] = std::move(cmd);
}
bool CLIService::hasCommand(const std::string &name) const {
    auto it = m_commands.find(name);
    return !(it == m_commands.end());
}
bool CLIService::execute(std::string rawString) {
    auto tokens = Tokenizer::tokenize(rawString);
    // / 2. Guard : Basic sanity checks
    if (tokens.empty() || tokens.front().type != TokenType::Word) {
        return false;
    }
    auto name = tokens.front().value;
    auto it = m_commands.find(name);
    if (it == m_commands.end()) {
        return false;
    }
    ICommand *cmd = it->second.get();
    return cmd->execute(tokens);
}