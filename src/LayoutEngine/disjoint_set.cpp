#include "disjoint_set.hpp"

bool DisjointSet::add(uint32_t x) {
    if (m_parent.count(x) > 0)
        return false;
    m_parent.insert({x, x});
    m_rank[x] = 0;
    return true;
}

int DisjointSet::find(uint32_t x) {
    if (m_parent.find(x) == m_parent.end())
        return -1;
    if (m_parent[x] != x)
        m_parent[x] = find(m_parent[x]); //compress path
    return m_parent[x];
}

bool DisjointSet::merge(uint32_t x, uint32_t y) {
    auto a = find(x);
    auto b = find(y);
    if (a == b)
        return;
    if (m_rank[a] > m_rank[b])
        std::swap(a, b);
    m_parent[b] = a;
    if (m_rank[a] == m_rank[b])
        m_rank[a]++;
}
bool DisjointSet::areDisjoint(uint32_t x, uint32_t y) {
    auto a = find(x);
    auto b = find(y);
    return a != b;
}
std::unordered_map<uint32_t, std::vector<uint32_t>> DisjointSet::groups() {
    std::unordered_map<uint32_t, std::vector<uint32_t>> result;
    for (auto &[node, _] : m_parent) {
        uint32_t root = find(node);
        result[root].push_back(node);
    }
    return result;
}
