#pragma once
#include <cstdint>
#include <string>


namespace GraphKeys {

inline std::string key(uint32_t from, uint32_t to) {
    return std::to_string(from) + "->" + std::to_string(to);
}
inline std::string key(uint32_t id) { return std::to_string(id); }

inline uint32_t extractFrom(const std::string &key) {
    auto arrowPos = key.find("->");
    if (arrowPos == std::string::npos)
        throw std::invalid_argument("Invalid edge key format: " + key);
    return std::stoul(key.substr(0, arrowPos));
}

inline uint32_t extractTo(const std::string &key) {
    auto arrowPos = key.find("->");
    if (arrowPos == std::string::npos)
        throw std::invalid_argument("Invalid edge key format: " + key);
    return std::stoul(key.substr(arrowPos + 2));
}

}; // namespace GraphKeys