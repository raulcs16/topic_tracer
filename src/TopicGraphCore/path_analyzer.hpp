#pragma once
#include "topic_graph.hpp"
#include <set>
#include <unordered_map>

namespace TG::PathAnalyzer {


struct BFSResult {
    std::vector<uint32_t> order;
    std::unordered_map<uint32_t, uint32_t> parent;
    std::unordered_map<uint32_t, int> dist;
};
BFSResult bfs(const TopicGraph &g, uint32_t start);

struct DFSResult {
    std::vector<uint32_t> order;
    std::unordered_map<uint32_t, uint32_t> parent;
    std::unordered_map<uint32_t, int> entry;
    std::unordered_map<uint32_t, int> exit;
};
DFSResult dfs(const TopicGraph &g, uint32_t start);

std::vector<uint32_t> topologicalSort(const TopicGraph &G);
double edgeWeight(EdgeType type);
std::vector<uint32_t> topicPath(std::unordered_map<uint32_t, int> parents, uint32_t dest);
std::vector<std::string> edgePath(std::vector<uint32_t> topicPath);
std::unordered_map<uint32_t, int> dijsktras(TopicGraph &graph,
                                            uint32_t start_id,
                                            uint32_t goal_id);

} // namespace TG::PathAnalyzer