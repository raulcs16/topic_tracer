#include "graph_keys.hpp"
#include "topic_graph.hpp"
#include <queue>
#include <vector>


using nodeDist = std::pair<uint32_t, double>;

namespace PathAnalyzer {
double edgeWeight(Edge_Type type) { return static_cast<int>(type) * 1.0; }
std::vector<uint32_t> topicPath(std::unordered_map<uint32_t, int> parents,
                                uint32_t dest) {
    if (parents.find(dest) == parents.end() || parents[dest] == -1) {
        return {};
    }
    uint32_t current = dest;
    std::vector<uint32_t> path{dest};
    while (parents[current] != -1) {
        current = parents[current];
        path.push_back(current);
    }
    std::reverse(path.begin(), path.end());
    return path;
}
std::vector<std::string> edgePath(std::vector<uint32_t> topicPath) {
    if (topicPath.size() < 2)
        return {};
    int i = 0;
    int j = 1;
    std::vector<std::string> edgeKeys;
    while (j < topicPath.size()) {
        std::string key = GraphKeys::key(topicPath[i], topicPath[j]);
        edgeKeys.push_back(key);
        j++;
        i++;
    }
    return edgeKeys;
}
std::unordered_map<uint32_t, int> dijsktras(TopicGraph &graph,
                                            uint32_t start_id,
                                            uint32_t goal_id) {
    auto cmp = [](const nodeDist &a, const nodeDist &b) { return a.second > b.second; };
    std::priority_queue<nodeDist, std::vector<nodeDist>, decltype(cmp)> pq(cmp);

    pq.emplace(start_id, 0);

    std::unordered_map<uint32_t, double> dist;
    std::unordered_map<uint32_t, int> parents;
    for (const auto &v : graph.topics()) {
        if (v.get()->id != start_id) {
            dist[v->id] = std::numeric_limits<double>::infinity();
            parents[v->id] = -1;
        }
    }
    dist[start_id] = 0;
    parents[start_id] = -1;
    pq.push({start_id, 0.0});
    while (!pq.empty()) {
        uint32_t v = pq.top().first;
        double d = pq.top().second;
        pq.pop();
        if (d > dist[v])
            continue;
        if (v == goal_id) {
            return parents;
        }
        for (const auto &e : graph.getOutEdges(v)) {
            uint32_t u = e.to;
            if (dist[u] > dist[v] + edgeWeight(e.type)) {
                dist[u] = edgeWeight(e.type) + dist[v];
                parents[u] = v;
                pq.emplace(u, dist[u]);
            }
        }
    }
    return parents;
}

} // namespace PathAnalyzer