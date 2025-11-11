#pragma once


#include "topic_graph_types.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>

class TopicGraph {
public:
    TopicGraph();

    //Topic API
    uint32_t addTopic(const std::string &name, Topic_Type type = Topic_Type::Concept);
    bool renameTopic(uint32_t id, const std::string &new_name);
    bool deleteTopic(uint32_t id);
    //getters
    std::shared_ptr<const Topic> getTopic(uint32_t id) const;
    std::shared_ptr<const Topic> getTopic(const std::string &name) const;
    std::vector<std::shared_ptr<Topic>> topics() const;

    //Edge API
    std::shared_ptr<const Edge> addEdge(uint32_t from, uint32_t to, Edge_Type type);
    std::shared_ptr<const Edge> addEdge(const std::string &topicA,
                                        const std::string &topicB,
                                        Edge_Type type);
    bool removeEdge(uint32_t from, uint32_t to);
    bool removeEdge(const std::string &topicA, const std::string &topicB);
    bool hasEdge(uint32_t from, uint32_t to);
    //gettters
    std::shared_ptr<const Edge> getEdge(std::string key);
    std::shared_ptr<const Edge> getEdge(uint32_t from, uint32_t to);
    std::vector<std::shared_ptr<Edge>> edges() const;


    std::vector<Edge> getOutEdges(uint32_t from) const;
    std::vector<Edge> getInEdges(uint32_t to) const;


private:
    std::unordered_map<uint32_t, Topic> m_topicMap;
    std::unordered_map<uint32_t, std::vector<Edge>> m_adjOutMap;
    std::unordered_set<std::string> m_keySet;


    size_t m_edge_count;
    uint32_t m_id_ref = 1;
};