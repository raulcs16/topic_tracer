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
    std::vector<std::shared_ptr<const Topic>> childrenOf(uint32_t id);
    std::vector<uint32_t> ancestorsOf(uint32_t id);
    std::vector<uint32_t> descendantsOf(uint32_t id);

    inline static bool isConcept(const Topic *t) { return t->type == TopicType::Concept; }
    inline static bool isConcrete(const Topic *t) {
        return t->type == TopicType::Concrete;
    }
    bool hasParent(uint32_t node, EdgeType parentType) const;
    inline static bool sameType(const Topic *a, const Topic *b) {
        return a->type == b->type;
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
    bool makesCycle(uint32_t from, uint32_t to, EdgeType type);
    bool dfsReachable(uint32_t start,
                      uint32_t target,
                      std::unordered_set<uint32_t> &visited);


    std::pair<const Topic *, const Topic *> normalizeJoin(const Topic *a,
                                                          const Topic *b,
                                                          EdgeType type);


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