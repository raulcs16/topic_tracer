#pragma once

#include "default_keys.hpp"
#include "token.hpp"
#include <map>
#include <optional>
#include <vector>

struct Argument {
    std::string name = "";
    std::string providerKey = TerminalKeys::None;
};

struct Flag {
    std::string providerKey = TerminalKeys::None;
    bool isBoolean = false;
    bool isRequired = false;
    std::string flag = "";
};

struct MapResult {
    std::map<std::string, std::string> values;
    bool success = true;
    Token error_token;
    std::string error_message;
};

class CommandSignature {

public:
    CommandSignature(const std::string &name);
    std::string name() const { return m_name; }
    void addPositionalArg(Argument arg);
    void addShortFlag(Flag flag);
    void addLongFlag(Flag flag);

    std::vector<std::string> keys(const std::vector<Token> &tokens) const;
    MapResult mapping(const std::vector<Token> &tokens) const;

private:
    void keyArgToken(std::vector<std::string> &keys, size_t &pos) const;
    void keyFlagGroup(std::vector<std::string> &keys,
                      const Token &token,
                      size_t &tokenIndex) const;
    void keyLongFlag(std::vector<std::string> &keys,
                     const Token &token,
                     size_t &tokenIndex) const;
    void mapEmptyToken() const {}
    void mapWordToken(MapResult &result, const Token &token, size_t &pos) const;
    void mapValueFlag(MapResult &result,
                      Flag f,
                      const std::vector<Token> &tokens,
                      size_t i) const;
    void mapLongFlag(MapResult &result,
                     const Token &token,
                     size_t &tokenIdx,
                     size_t totalTokens) const;
    std::vector<Flag> parseFlagGroup(MapResult &result, const Token &token) const;
    void resultError(MapResult &result,
                     const Token &token,
                     const std::string &message) const;

private:
    std::string m_name;
    std::vector<Argument> m_args;

    size_t m_req_flags = 0;

    std::map<char, Flag> m_reqShortFlags;
    std::map<std::string, Flag> m_reqLongFlags;

    std::map<std::string, Flag> m_shortFlags;
    std::map<std::string, Flag> m_longFlags;
};