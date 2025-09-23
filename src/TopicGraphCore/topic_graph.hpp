#pragma once

#include "edge_type.hpp"
#include "topic.hpp"
#include <unordered_map>
#include <vector>

class TopicGraph {
public:
    TopicGraph();


    void addTopic(const std::string &name, Topic_Type type = Topic_Type::Concept);
    void removeTopic(uint32_t id);

    //get
    std::shared_ptr<const Topic> getTopic(uint32_t id) const;
    std::shared_ptr<const Topic> getTopic(const std::string &name) const;

    void addEdge(uint32_t from, uint32_t to, Edge_Type type);
    bool hasEdge(uint32_t from, uint32_t to) const;
    void removeEdge(uint32_t from, uint32_t to);

    //todo: look into shared_ptr for Edges
    std::vector<Edge> getOutEdges(uint32_t from) const;
    // std::shared_ptr<const Edge> getInEdges(uint32_t to) const;

    size_t numTopics() const;
    size_t numEdges() const;


private:
    std::unordered_map<uint32_t, std::shared_ptr<Topic>> m_topicMap;
    std::unordered_map<uint32_t, std::vector<Edge>> m_adjOutMap;

    size_t m_edge_count;
    uint32_t m_id_ref;
};