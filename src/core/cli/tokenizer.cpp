#include "tokenizer.h"

std::vector<Token> Tokenizer::tokenize(const std::string &input) {
    std::vector<Token> tokens;

    size_t len = input.size();
    size_t i = 0;

    while (i < len) {
        // 1. Skip Whitespace
        if (std::isspace(static_cast<unsigned char>(input[i]))) {
            i++;
            continue;
        }
        size_t start = i;
        bool quote = false;
        bool doubleQuote = false;
        TokenType type = TokenType::Word;
        if (input[i] == '-') {
            type = TokenType::FlagGroup;
            i++;
            if (i < len && input[i] == '-') {
                type = TokenType::LongFlag;
                i++; // Move past second '-'
            }
        } else if (input[i] == '=') {
            i++;
        } else if (input[i] == '\"' || input[i] == '\'') {
            quote = true;
            if (input[i] == '\"') {
                doubleQuote = true;
            }
            i++;
        }
        size_t crop = i;

        while (i < len) {
            char c = input[i];
            if (quote) {
                if (doubleQuote && c == '\"')
                    break;
                if (!doubleQuote && c == '\'')
                    break;
            } else {
                if (std::isspace(static_cast<unsigned char>(c)) || c == '=' ||
                    c == '\'' || c == '\"') {
                    break; // Stop word at whitespace, assignment, or quotes
                }
            }
            i++;
        }

        size_t end = i;
        // FIX #1: Length calculation for substr is (end - crop)
        std::string value = input.substr(crop, end - crop);
        if (value.empty())
            continue;
        tokens.push_back({type, value, start, end});
    }
    if (input.back() == ' ') {
        tokens.push_back({TokenType::Empty, " ", input.size() - 1, input.size()});
    }
    return tokens;
}