#pragma once

#include <string>

enum class TokenType {
    Word,      // any standalone string
    FlagGroup, // "-[word]"   (-t, -rf, -xyz)
    LongFlag,  //starts with "--[word]"  (--type)
    Seperator, // no more flags "--"
    Empty,     //" "
};

struct Token {
    TokenType type;
    std::string value;
    size_t start;
    size_t end;
};