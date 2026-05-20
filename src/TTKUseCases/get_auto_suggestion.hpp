#pragma once
#include "cli_service.hpp"
#include "suggestion_engine.hpp"
#include "terminal_out.hpp"
#include "tokenizer.hpp"

class GetAutoSuggestion {
public:
    GetAutoSuggestion(CLIService *cli, SuggestionEngine *se, TerminalOut *port)
        : m_cli{cli}, m_se{se}, m_port{port} {}
    void execute(const std::string &raw_input) {
        auto tokens = Tokenizer::tokenize(raw_input);
        if (tokens.empty()) {
            m_port->presentSuggestion("");
        }
        auto cmd = m_cli->getCommand(tokens);
        if (!cmd) {
            m_port->presentLine("no matching command");
        }
        auto def = cmd->getCommandSignature();
        auto keys = def.keys(tokens);
        if (keys.empty()) {
            m_port->presentSuggestion("");
        }
        auto matches = m_se->matches(keys.back(), tokens.back().value);
        if (matches.empty()) {
            m_port->presentSuggestion("");
        } else if (matches.size() > 1) {
            m_port->presentList(matches);
        } else {
            auto replacement = fromTokens(tokens, matches[0]);
            m_port->presentSuggestion(replacement);
        }
    }

private:
    CLIService *m_cli;
    SuggestionEngine *m_se;
    TerminalOut *m_port;
};

std::string fromTokens(std::vector<Token> tokens, std::string replacement) {
    if (tokens.empty())
        return replacement;
    tokens.back().value = replacement;
    std::string suggestion = "";
    for (auto token : tokens) {
        suggestion.append(token.value);
        suggestion.append(" ");
    }
    return suggestion;
}
