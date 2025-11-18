#pragma once


#include "topic_graph_types.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>

class TopicGraph {
public:
    TopicGraph();
    //Topic API
    uint32_t addTopic(const std::string &name, TopicType type = TopicType::Concept);
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

private:
    uint32_t nextId();
    std::string makeKey(uint32_t from, uint32_t to);

private:
    std::unordered_map<uint32_t, std::shared_ptr<Topic>> m_topicMap;
    std::unordered_map<std::string, std::shared_ptr<Edge>> m_edgeMap;

    std::unordered_map<uint32_t, std::vector<std::shared_ptr<Edge>>> m_adjOutMap;
    std::unordered_map<uint32_t, std::vector<std::shared_ptr<Edge>>> m_adjInMap;

    uint32_t m_id_ref = 1;
};