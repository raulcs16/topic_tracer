#pragma once
#include "topic_graph.hpp"
#include <set>
#include <unordered_map>

namespace PathAnalyzer {


double edgeWeight(Edge_Type type);
std::vector<uint32_t> topicPath(std::unordered_map<uint32_t, int> parents, uint32_t dest);
std::vector<std::string> edgePath(std::vector<uint32_t> topicPath);
std::unordered_map<uint32_t, int> dijsktras(TopicGraph &graph,
                                            uint32_t start_id,
                                            uint32_t goal_id);

} // namespace PathAnalyzer