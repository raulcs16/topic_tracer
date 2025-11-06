#include "topic_graph.hpp"
#include <queue>


using nodeDist = std::pair<uint32_t, double>;

namespace PathAnalyzer {
double edgeWeight(Edge_Type type) {
    switch (type) {
    case Edge_Type::AlternativeTo: return 0.8;
    case Edge_Type::ComposedOf: return 2.0;
    case Edge_Type::DependsOn: return 3.0;
    case Edge_Type::Example: return 1.0;
    case Edge_Type::RelatedTo: return 1.5;
    default: return 1.0;
    }
}
std::unordered_map<uint32_t, int> dijsktras(TopicGraph &graph,
                                            uint32_t start_id,
                                            uint32_t goal_id) {
    std::priority_queue<nodeDist> pq;
    pq.push({start_id, 0});

    std::unordered_map<uint32_t, double> dist;
    std::unordered_map<uint32_t, int> parents;
    for (const auto &v : graph.topics()) {
        if (v.get()->id != start_id) {
            dist.insert({v.get()->id, std::numeric_limits<double>::infinity()});
            parents.insert({v.get()->id, -1});
        }
    }
    dist[start_id] = 0;
    while (!pq.empty()) {
        uint32_t v = pq.top().first;
        pq.pop();
        if (v == goal_id) {
            parents[goal_id] = v;
            return parents;
        }
        for (const auto &e : graph.getOutEdges(start_id)) {
            uint32_t u = e.to;
            if (dist[u] > dist[v] + edgeWeight(e.type)) {
                dist[u] = edgeWeight(e.type) + dist[v];
                parents[u] = v;
                pq.push({u, dist[u]});
            }
        }
    }
    return parents;
}

} // namespace PathAnalyzer