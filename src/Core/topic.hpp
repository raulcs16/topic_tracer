#pragma once
#include <cstdint>
#include <string>


enum class Topic_Type {
    concept,
    concrete
};


class Topic {

public:
    Topic(uint32_t id, std::string name, Topic_Type type) : id(id), name(std::move(name)), type(type), covered(false) {}
    uint32_t getId() const { return id; }
    const std::string &getName() const { return name; }
    bool isCovered() const { return covered; }
    void setCovered(bool is_covered) { covered = is_covered; }
    Topic_Type getType() const { return type; }

private:
    uint32_t id;
    std::string name;
    bool covered;
    Topic_Type type;
};