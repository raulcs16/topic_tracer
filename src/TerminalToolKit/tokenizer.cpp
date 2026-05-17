#include "tokenizer.hpp"

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
        TokenType type = TokenType::Word;
        if (input[i] == '-') {
            type = TokenType::FlagGroup;
            i++;
            if (i < len && input[i] == '-') {
                type = TokenType::LongFlag;
                i++; // Move past second '-'
            }
        }
        size_t crop = i;
        while (i < len && !std::isspace(static_cast<unsigned char>(input[i]))) {
            i++;
        }

        size_t end = i - 1;

        std::string value = input.substr(crop, i - crop);
        if (value == "=")
            continue;
        auto it = std::find(value.begin(), value.end(), '=');
        if (it != value.end()) {
            auto dist = std::distance(value.begin(), it);
            if (dist == 0) //=str
                value = value.substr(dist + 1);
            else if (dist == value.size() - 1) { // str=
                value = value.substr(0, value.size() - 1);
            } else { //str=str
                std::string value2 = value.substr(dist);
                end -= dist;
                value = value.substr(0, dist);
                tokens.push_back({type, value, start, end});
                tokens.push_back({TokenType::Word, value2, start + dist, end});
                continue;
            }
        }
        tokens.push_back({type, value, start, end});
    }
    if (input.empty() || std::isspace(static_cast<unsigned char>(input.back()))) {
        size_t pos = (input.size());
        tokens.push_back({TokenType::Empty, "", pos, pos});
    }
    return tokens;
}