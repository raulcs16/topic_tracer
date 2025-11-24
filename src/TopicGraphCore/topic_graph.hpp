#pragma once


#include "topic_graph_types.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>

class TopicGraph {
public:
    TopicGraph();
    //Topic API
    std::shared_ptr<const Topic> addTopic(const std::string &name,
                                          TopicType type = TopicType::Concept);
    bool renameTopic(uint32_t id, const std::string &new_name);
    bool deleteTopic(uint32_t id);
    //getters
    std::shared_ptr<const Topic> getTopic(uint32_t id) const;
    std::shared_ptr<const Topic> getTopic(const std::string &name) const;
    std::vector<std::shared_ptr<const Topic>> topics() const;

    //Edge API
    std::shared_ptr<const Edge> addEdge(uint32_t from, uint32_t to, EdgeType type);
    std::shared_ptr<const Edge> addEdge(const std::string &topicA,
                                        const std::string &topicB,
                                        EdgeType type);
    std::shared_ptr<const Edge> addEdge(const Topic *a, const Topic *b, EdgeType type);

    bool removeEdge(uint32_t from, uint32_t to);
    bool removeEdge(const std::string &topicA, const std::string &topicB);
    bool hasEdge(const std::string &key);
    //gettters
    std::shared_ptr<const Edge> getEdge(const std::string &key) const;
    std::shared_ptr<const Edge> getEdge(uint32_t from, uint32_t to) const;

    std::vector<std::shared_ptr<Edge>> edges() const;
    std::vector<std::shared_ptr<Edge>> getOutEdges(uint32_t from) const;
    std::vector<std::shared_ptr<Edge>> getInEdges(uint32_t to) const;

    //Graph Data
    size_t edgeCount() const { return m_edgeMap.size(); }
    size_t topicCount() const { return m_topicMap.size(); }

    std::shared_ptr<const Topic> parent(uint32_t);

    inline static bool isConcept(const Topic *t) { return t->type == TopicType::Concept; }
    inline static bool isConcrete(const Topic *t) {
        return t->type == TopicType::Concrete;
    }
    inline static bool sameType(const Topic *a, const Topic *b) {
        return a->type == b->type;
    }
    inline bool hasParent(uint32_t node, EdgeType parentType) const {
        auto it = m_adjInMap.find(node);
        if (it == m_adjInMap.end())
            return false;

        for (auto &e : it->second) {
            if (e->type == parentType)
                return true;
        }
        return false;
    }
    inline bool sameParent(const Topic *a, const Topic *b) {
        auto pa = parent(a->id);
        auto pb = parent(b->id);
        return pa != nullptr && pa == pb;
    }
    inline bool sameParent(uint32_t a, uint32_t b) {
        auto pa = parent(a);
        auto pb = parent(b);
        return pa != nullptr && pa == pb;
    }


    inline bool makesCycle(uint32_t from, uint32_t to, EdgeType type) {
        // Only enforce for hierarchical edges
        if (type != EdgeType::ComposedOf && type != EdgeType::Example)
            return false;

        // Perform DFS from `to` to see if we can reach `from`
        std::unordered_set<uint32_t> visited;
        return dfsReachable(to, from, visited);
    }
    inline bool dfsReachable(uint32_t start,
                             uint32_t target,
                             std::unordered_set<uint32_t> &visited) {
        if (start == target)
            return true;
        visited.insert(start);

        for (auto &edge : m_adjOutMap[start]) {
            if (edge->type != EdgeType::ComposedOf && edge->type != EdgeType::Example)
                continue;

            uint32_t next = edge->to;
            if (!visited.count(next) && dfsReachable(next, target, visited))
                return true;
        }
        return false;
    }


    inline std::pair<const Topic *, const Topic *> normalizeJoin(const Topic *a,
                                                                 const Topic *b,
                                                                 EdgeType type) {
        if (type != EdgeType::Example)
            return {a, b};
        if (a->type == TopicType::Concept && b->type == TopicType::Concrete)
            return {a, b};
        if (b->type == TopicType::Concept && a->type == TopicType::Concrete)
            return {b, a}; // swap
        return {a, b};
    }

private:
    uint32_t nextId();
    std::string makeKey(uint32_t from, uint32_t to);

private:
    std::unordered_map<uint32_t, std::shared_ptr<Topic>> m_topicMap;
    std::unordered_map<std::string, std::shared_ptr<Edge>> m_edgeMap;

    std::unordered_map<uint32_t, std::vector<std::shared_ptr<Edge>>> m_adjOutMap;
    std::unordered_map<uint32_t, std::vector<std::shared_ptr<Edge>>> m_adjInMap;

    using Rule = std::function<bool(const Topic *, const Topic *)>;
    std::unordered_map<EdgeType, std::vector<Rule>> m_validators;
    uint32_t m_id_ref = 1;
};