#include "command_signature.hpp"
#include "default_keys.hpp"

CommandSignature::CommandSignature(const std::string &name) : m_name(name) {}

void CommandSignature::addPositionalArg(Argument arg) { m_args.push_back(arg); }
void CommandSignature::addShortFlag(Flag flag) {
    if (flag.flag == "")
        return;
    if (m_shortFlags.contains(flag.flag))
        return;
    m_shortFlags[flag.flag] = flag;
    if (flag.isRequired)
        m_req_flags++;
}
void CommandSignature::addLongFlag(Flag flag) {
    if (flag.flag == "")
        return;
    if (m_longFlags.contains(flag.flag))
        return;
    m_longFlags[flag.flag] = flag;
    if (flag.isRequired)
        m_req_flags++;
}
std::vector<std::string> CommandSignature::keys(const std::vector<Token> &tokens) const {
    std::vector<std::string> keys;
    if (tokens.empty())
        return keys;
    // Command Root
    size_t tokenIdx = 0;
    size_t positionalCount = 0;

    bool includes_name = false;
    while (tokens[tokenIdx].type == TokenType::Word) {
        keys.push_back(TerminalKeys::Cmd);
        if (tokens[tokenIdx].value == m_name) {
            includes_name = true;
            tokenIdx++;
            break;
        }
        tokenIdx++;
    }
    if (!includes_name) {
        keys.push_back(TerminalKeys::Error);
        return keys;
    }

    while (tokenIdx < tokens.size()) {
        auto token = tokens.at(tokenIdx);
        switch (token.type) {
        case TokenType::Word:
        case TokenType::Empty: keyArgToken(keys, positionalCount); break;
        case TokenType::FlagGroup: keyFlagGroup(keys, token, tokenIdx); break;
        case TokenType::LongFlag: keyLongFlag(keys, token, tokenIdx); break;
        default: break;
        }
        tokenIdx++;
    }
    return keys;
}
void CommandSignature::keyArgToken(std::vector<std::string> &keys, size_t &pos) const {
    if (pos >= m_args.size())
        return;
    auto arg = m_args.at(pos);
    keys.push_back(arg.providerKey);
    pos++;
}
void CommandSignature::keyFlagGroup(std::vector<std::string> &keys,
                                    const Token &token,
                                    size_t &tokenIndex) const {
    MapResult result;
    auto flags = parseFlagGroup(result, token);
    if (!result.success) {
        keys.push_back(TerminalKeys::Error);
    }
    if (flags.size() == 1) {
        if (flags[0].isBoolean) {
            keys.push_back(TerminalKeys::BoolFlags);
        } else {
            keys.push_back(TerminalKeys::ValueFlag);
            keys.push_back(flags[0].providerKey);
            tokenIndex++;
        }
    } else {
        auto it = flags.begin();
        bool error_found = false;
        for (; it < flags.end() - 1; it++) {
            if (!it->isBoolean) {
                error_found = true;
                keys.push_back(TerminalKeys::Error);
            }
        }
        //check last one
        if (it->isBoolean && !error_found) {
            keys.push_back(TerminalKeys::BoolFlags);
        } else {
            keys.push_back(TerminalKeys::ValueFlag);
            keys.push_back(flags[0].providerKey);
            tokenIndex++;
        }
    }
}
void CommandSignature::keyLongFlag(std::vector<std::string> &keys,
                                   const Token &token,
                                   size_t &tokenIndex) const {
    auto it = m_longFlags.find(token.value);
    if (it == m_longFlags.end()) {
        keys.push_back(TerminalKeys::Error);
        return;
    }
    auto flag = it->second;
    if (flag.isBoolean) {
        keys.push_back(TerminalKeys::BoolFlags);
    } else {
        keys.push_back(TerminalKeys::ValueFlag);
        keys.push_back(flag.providerKey);
        tokenIndex++; //highjack this spot
    }
}
MapResult CommandSignature::mapping(const std::vector<Token> &tokens) const {
    MapResult result{};
    if (tokens.empty()) {
        result.success = false;
        return result;
    }
    if (tokens[0].value != m_name) {
        resultError(result, tokens[0], "incorrect command");
        return result;
    }

    result.values[TerminalKeys::Cmd] = m_name;

    size_t tokenIdx = 1;
    size_t positionalCount = 0;
    size_t reqFlagCount = 0;
    std::map<std::string, size_t> flag_counter;

    while (tokenIdx < tokens.size()) {
        auto token = tokens.at(tokenIdx);
        if (token.type == TokenType::Empty) {
        } else if (token.type == TokenType::Word) {
            if (positionalCount < m_args.size()) {
                mapWordToken(result, token, positionalCount);
            } else {
                resultError(result, token, "unrecognized token");
                return result;
            }
        } else if (token.type == TokenType::FlagGroup) {
            auto flags = parseFlagGroup(result, token);
            if (!result.success)
                return result;
            if (flags.size() == 1) {
                if (flag_counter[flags[0].flag] > 0) {
                    //already counted this flag
                    resultError(result, token, "flag(s) were counted twice");
                    return result;
                }
                flag_counter[flags[0].flag]++;
                if (flags[0].isBoolean) {
                    result.values[token.value] = TerminalKeys::BoolFlags;
                } else {
                    tokenIdx++;
                    mapValueFlag(result, flags[0], tokens, tokenIdx);
                    if (!result.success) {
                        return result;
                    }
                    if (flags[0].isRequired) {
                        reqFlagCount++;
                    }
                }
            } else {
                //every flag but maybe the end should be boolean
                auto it = flags.begin();
                for (; it < flags.end() - 1; it++) {
                    if (flag_counter[it->flag] > 0) {
                        resultError(result, token, "flag(s) were counted twice");
                        return result;
                    }
                    flag_counter[it->flag]++;
                    if (!it->isBoolean) {
                        resultError(result,
                                    token,
                                    "mixed in value flag with booleans" + it->flag);
                        return result;
                    }
                    result.values[it->flag] = TerminalKeys::BoolFlags;
                }
                if (flag_counter[it->flag] > 0) {
                    resultError(result, token, "flag(s) were counted twice");
                    return result;
                }
                flag_counter[it->flag]++;
                if (it->isBoolean) {
                    result.values[it->flag] = TerminalKeys::BoolFlags;
                } else {
                    tokenIdx++;
                    mapValueFlag(result, *(it), tokens, tokenIdx);
                    if (!result.success) {
                        return result;
                    }
                    if (flags[0].isRequired) {
                        reqFlagCount++;
                    }
                }
            }
        } else if (token.type == TokenType::LongFlag) {
            if (!m_longFlags.contains(token.value)) {
                resultError(result, token, "unknown long flag" + token.value);
                return result;
            }
            auto f = m_longFlags.at(token.value);
            if (flag_counter[f.flag] > 0) {
                //already counted this flag
                resultError(result, token, "flag(s) were counted twice");
                return result;
            }
            flag_counter[f.flag]++;
            if (f.isBoolean) {
                result.values[f.flag] = TerminalKeys::BoolFlags;
            } else {
                tokenIdx++;
                mapValueFlag(result, f, tokens, tokenIdx);
                if (!result.success) {
                    return result;
                }
                if (f.isRequired) {
                    reqFlagCount++;
                }
            }
        }
        tokenIdx++;
    }
    if (reqFlagCount != m_req_flags) {
        resultError(result, tokens.back(), "missing required flags");
    }
    if (positionalCount != m_args.size()) {
        resultError(result, tokens.back(), "missing arguements");
    }

    return result;
}

void CommandSignature::resultError(MapResult &result,
                                   const Token &error_token,
                                   const std::string &message) const {
    result.error_token = error_token;
    result.success = false;
    result.error_message = message;
}
void CommandSignature::mapWordToken(MapResult &result,
                                    const Token &token,
                                    size_t &pos) const {
    if (pos >= m_args.size())
        return;
    auto key = m_args[pos].name;
    result.values[key] = token.value;
    pos++;
}
std::vector<Flag> CommandSignature::parseFlagGroup(MapResult &result,
                                                   const Token &token) const {
    std::vector<Flag> flags;
    for (auto &f : token.value) {
        std::string flagStr(1, f); //
        if (!m_shortFlags.contains(flagStr)) {
            resultError(result, token, "unknown flag:" + flagStr);
            break;
        }
        flags.push_back(m_shortFlags.at(flagStr));
    }
    return flags;
}

void CommandSignature::mapValueFlag(MapResult &result,
                                    Flag f,
                                    const std::vector<Token> &tokens,
                                    size_t i) const {
    if (i >= tokens.size()) {
        resultError(result, tokens[i], "missing argument for flag:" + f.flag);
        return;
    }
    auto valueToken = tokens[i];
    if (valueToken.type != TokenType::Word) {
        resultError(result, valueToken, "incorrect type");
        return;
    }
    result.values[f.flag] = valueToken.value;
}

void CommandSignature::mapLongFlag(MapResult &result,
                                   const Token &token,
                                   size_t &tokenIdx,
                                   size_t totalTokens) const {
    auto it = m_longFlags.find(token.value);
    if (it == m_longFlags.end()) {
        return;
    }
    auto flag = it->second;
    if (flag.isBoolean) {
        result.values[token.value] = "bool";
    } else {
        tokenIdx++; //highjack this spot
        if (tokenIdx >= totalTokens) {
            return;
        }
        result.values[token.value] = token.value;
    }
}