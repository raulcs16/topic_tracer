#pragma once

#include "cli_service.hpp"
#include "terminal_out.hpp"
#include "tokenizer.hpp"

class HandleInput {
public:
    HandleInput(CLIService *cli, TerminalOut *out) : m_cli{cli}, m_out{out} {}

    void execute(const std::string &raw_input) {
        auto tokens = Tokenizer::tokenize(raw_input);
        auto cmd = m_cli->getCommand(tokens);
        if (!cmd) {
            m_out->presentLine("cmd not recognized");
        }
        auto sucess = cmd->execute(tokens);
        if (!sucess) {
            m_out->presentLine("error in cmd signature");
        }
        m_out->presentLine(raw_input);
    }

private:
    CLIService *m_cli;
    TerminalOut *m_out;
};