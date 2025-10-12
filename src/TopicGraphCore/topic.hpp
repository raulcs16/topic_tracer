#pragma once
#include <cstdint>
#include <string>

enum class Topic_Type {
    Concept,
    Concrete
};

struct Topic {
    uint32_t id;
    std::string name;
    bool covered;
    Topic_Type type;
};