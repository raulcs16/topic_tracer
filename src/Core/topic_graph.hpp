#pragma once

#include "topic.hpp"
#include <unordered_map>
#include <vector>

enum class Edge_Type {
    depends_on,
    example,
    composed_of,
    alternative_to,
    related_to,
};
struct Edge {
    uint32_t targetId;
    Edge_Type type;
};

class TopicGraph {
public:
    TopicGraph();

    // Topic operations
    void addTopic(std::string &name, Topic_Type topic_type);
    void removeTopic(uint32_t id);
    std::shared_ptr<Topic> getTopic(uint32_t id) const;
    std::shared_ptr<Topic> getTopic(const std::string &name) const;
    std::vector<uint32_t> getAllTopicIds() const;

    // Edge operations
    void addEdge(uint32_t from, uint32_t to, Edge_Type type);
    bool hasEdge(uint32_t from, uint32_t to) const;
    bool hasEdge(uint32_t from, uint32_t to, Edge_Type type) const;
    void removeEdge(uint32_t from, uint32_t to);
    std::vector<Edge> getOutgoingEdges(uint32_t from) const;
    std::vector<Edge> getIncomingEdges(uint32_t to) const;
    std::vector<uint32_t> getNeighbors(uint32_t from, Edge_Type type = Edge_Type::depends_on) const;

    // Utilities
    size_t numTopics() const;
    size_t numEdges() const;

private:
    std::unordered_map<uint32_t, std::shared_ptr<Topic>> topics;
    std::unordered_map<uint32_t, std::vector<Edge>> adjOutMap;
    size_t edge_count;
    uint32_t id_ref;
};
