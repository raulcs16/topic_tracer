#pragma once

#pragma once
#include "command_signature.hpp"
#include "token.hpp"


class ICommand {
public:
    ICommand(std::string name) : m_signature(name) {}
    virtual ~ICommand() = default;
    bool execute(const std::vector<Token> &tokens) const {
        if (tokens.empty())
            return false;
        if (tokens.size() > 1 && tokens[1].type == TokenType::Word) {
            std::string nextWord = tokens[1].value;
            auto it = m_subcommands.find(nextWord);
            if (it == m_subcommands.end())
                return false;
            std::vector<Token> subTokens(tokens.begin() + 1, tokens.end());
            return it->second->execute(subTokens);
        }
        auto result = m_signature.mapping(tokens);
        if (!result.success)
            return false;
        return doExecute(result.values);
    }
    void registerSubCommand(std::unique_ptr<ICommand> child) {
        if (!child)
            return;
        m_subcommands[child->getCommandSignature().name()] = std::move(child);
    }
    const CommandSignature &getCommandSignature() const { return m_signature; }
    const ICommand *getSubCommand(const std::vector<Token> &tokens) const {
        if (tokens.empty())
            return this;
        if (tokens.front().type == TokenType::Word) {
            std::string nextWord = tokens.front().value;
            auto it = m_subcommands.find(nextWord);
            if (it != m_subcommands.end()) {
                std::vector<Token> subTokens{tokens.begin() + 1, tokens.end()};
                return it->second->getSubCommand(subTokens);
            }
        }
        return this;
    }

protected:
    virtual bool doExecute(const std::map<std::string, std::string> &values) const = 0;
    CommandSignature m_signature;
    std::map<std::string, std::unique_ptr<ICommand>> m_subcommands;
};