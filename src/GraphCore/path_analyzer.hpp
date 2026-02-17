#pragma once
#include "graph.hpp"
#include <set>
#include <unordered_map>

namespace TG::PathAnalyzer {

using ParentMap = std::unordered_map<uint32_t, int32_t>;
struct BFSResult {
    std::vector<uint32_t> order;
    std::unordered_map<uint32_t, uint32_t> parent;
    std::unordered_map<uint32_t, int> dist;
};

struct DFSResult {
    std::vector<uint32_t> order;
    std::unordered_map<uint32_t, uint32_t> parent;
    std::unordered_map<uint32_t, int> entry;
    std::unordered_map<uint32_t, int> exit;
};

BFSResult bfs(const Graph &g, uint32_t start);

DFSResult dfs(const Graph &g, uint32_t start);

std::vector<uint32_t> topologicalSort(const Graph &G);
double edgeWeight(EdgeType type);
std::vector<uint32_t> topicPath(std::unordered_map<uint32_t, int> parents, uint32_t dest);
std::vector<std::string> edgePath(std::vector<uint32_t> topicPath);
std::unordered_map<uint32_t, int> dijsktras(Graph &graph,
                                            uint32_t start_id,
                                            uint32_t goal_id);

} // namespace TG::PathAnalyzer