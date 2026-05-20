#include "cli_service.hpp"
#include "tokenizer.hpp"


CLIService::CLIService() {}
void CLIService::registerCommand(std::unique_ptr<ICommand> cmd) {
    if (!cmd)
        return;
    m_root.registerSubCommand(std::move(cmd));
    m_cmd_count++;
}
bool CLIService::execute(const std::string &rawString) {
    auto tokens = Tokenizer::tokenize(rawString);
    auto cmd = getCommand(tokens);
    return cmd->execute(tokens);
}
const ICommand *CLIService::getCommand(const std::vector<Token> &tokens) {
    if (tokens.empty()) {
        return &m_root;
    }
    return m_root.getSubCommand(tokens);
}