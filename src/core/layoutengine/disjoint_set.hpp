#pragma once
#include <cstdint>
#include <set>
#include <unordered_map>
#include <vector>

class DisjointSet {
public:
    bool add(uint32_t x);
    int find(uint32_t x);
    bool merge(uint32_t x, uint32_t y);
    bool areDisjoint(uint32_t x, uint32_t y);
    std::unordered_map<uint32_t, std::vector<uint32_t>> groups();

private:
    std::unordered_map<uint32_t, uint32_t> m_parent;
    std::unordered_map<uint32_t, uint32_t> m_rank;
};