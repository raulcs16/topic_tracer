#include "edge_semantic.hpp"
#include "graph_keys.hpp"
#include "path_analyzer.hpp"
#include "topic_graph.hpp"
#include <iostream>
#include <queue>
#include <vector>


using nodeDist = std::pair<uint32_t, double>;


namespace TG::PathAnalyzer {
BFSResult bfs(const TopicGraph &g, uint32_t start) {
    BFSResult result;
    std::queue<uint32_t> q;
    std::unordered_set<uint32_t> visited;

    visited.insert(start);
    q.push(start);
    result.parent[start] = UINT32_MAX;
    result.dist[start] = 0;

    while (!q.empty()) {
        uint32_t u = q.front();
        q.pop();
        result.order.push_back(u);
        //traverse hirarichal edges
        for (auto &edge : g.getOutEdges(u)) {
            if (TG::Semantic::of(edge->type) !=
                TG::Semantic::EdgeSemantic::Hierarchical) {
                continue;
            }
            uint32_t v = edge->to;
            if (!visited.count(v)) {
                visited.insert(v);
                q.push(v);
                result.parent[v] = u;
                result.dist[v] = result.dist[u] + 1;
            }
        }
    }
    return result;
}
void dfsVist(const TopicGraph &g,
             uint32_t u,
             std::unordered_set<uint32_t> &visited,
             std::unordered_map<uint32_t, uint32_t> &parent,
             std::vector<uint32_t> &order,
             std::unordered_map<uint32_t, int> &entry,
             std::unordered_map<uint32_t, int> &exit,
             int &time) {
    visited.insert(u);
    entry[u] = time++;
    order.push_back(u);
    for (auto &edge : g.getOutEdges(u)) {
        if (TG::Semantic::of(edge->type) != TG::Semantic::EdgeSemantic::Hierarchical) {
            continue;
        }
        uint32_t v = edge->to;
        if (!visited.count(v)) {
            parent[v] = u;
            dfsVist(g, v, visited, parent, order, entry, exit, time);
        }
    }
    exit[u] = time++;
}

DFSResult dfs(const TopicGraph &g, uint32_t start) {
    DFSResult result;
    std::unordered_set<uint32_t> visited;
    int time = 0;

    result.parent[start] = UINT32_MAX;

    dfsVist(g,
            start,
            visited,
            result.parent,
            result.order,
            result.entry,
            result.exit,
            time);


    return result;
}
std::vector<uint32_t> topologicalSort(const TopicGraph &g) {
    std::unordered_map<uint32_t, int> inDegree;
    std::queue<uint32_t> q;
    std::vector<uint32_t> sorted;

    // Initialize in-degree counts for hierarchical edges
    for (auto &topic : g.topics()) {
        uint32_t id = topic->id;
        inDegree[id] = 0;
    }
    for (auto &topic : g.topics()) {
        uint32_t u = topic->id;
        for (auto &edge : g.getOutEdges(u)) {
            if (TG::Semantic::of(edge->type) !=
                TG::Semantic::EdgeSemantic::Hierarchical) {
                continue;
            }
            inDegree[edge->to]++;
        }
    }

    // Start with nodes of in-degree 0
    for (auto &[id, deg] : inDegree) {
        if (deg == 0)
            q.push(id);
    }

    while (!q.empty()) {
        uint32_t u = q.front();
        q.pop();
        sorted.push_back(u);

        for (auto &edge : g.getOutEdges(u)) {
            if (TG::Semantic::of(edge->type) !=
                TG::Semantic::EdgeSemantic::Hierarchical) {
                continue;
            }

            uint32_t v = edge->to;
            inDegree[v]--;
            if (inDegree[v] == 0)
                q.push(v);
        }
    }
    // If sorted size != total nodes, there is a cycle
    if (sorted.size() != g.topicCount()) {
        throw std::runtime_error("Cycle detected in hierarchical edges");
    }

    return sorted;
}
double edgeWeight(EdgeType type) { return static_cast<int>(type) * 1.0; }
std::vector<uint32_t> topicPath(std::unordered_map<uint32_t, int> parents,
                                uint32_t dest) {
    if (parents.find(dest) == parents.end() || parents[dest] == -1) {
        for (const auto [p, v] : parents) {
            std::cout << p << ":" << v << std::endl;
        }
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
        dist[v->id] = std::numeric_limits<double>::infinity();
        parents[v->id] = -1;
    }
    dist[start_id] = 0;
    while (!pq.empty()) {
        uint32_t v = pq.top().first;
        double d = pq.top().second;
        pq.pop();
        if (v == goal_id) {
            return parents;
        }
        for (const auto &e : graph.getOutEdges(v)) {
            uint32_t u = e->to;
            double w = static_cast<int>(e->type) * 1.0;
            if (dist[u] > dist[v] + w) {
                dist[u] = w + dist[v];
                parents[u] = v;
                pq.emplace(u, dist[u]);
            }
        }
    }
    return parents;
}
} // namespace TG::PathAnalyzer