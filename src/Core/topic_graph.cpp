#include "topic.hpp"
#include "topic_graph.hpp"

TopicGraph::TopicGraph() {
    edge_count = 0;
    id_ref = 1;
}


void TopicGraph::addTopic(std::string &name, Topic_Type topic_type) {
    //TODO trim name


    if (getTopic(name) != nullptr)
        return;
    auto newTopic = std::make_shared<Topic>(id_ref++, name, topic_type);
    topics[newTopic->getId()] = newTopic;
    adjOutMap[newTopic->getId()] = {};
}
void TopicGraph::removeTopic(uint32_t id) {}
std::shared_ptr<Topic> TopicGraph::getTopic(uint32_t id) const {
    auto it = topics.find(id);
    if (it != topics.end())
        return it->second;
    return nullptr;
}
std::shared_ptr<Topic> TopicGraph::getTopic(const std::string &name) const {
    for (const auto &[id, topic] : topics) {
        if (topic->getName() == name) {
            return topic;
        }
    }
    return nullptr;
}
std::vector<uint32_t> TopicGraph::getAllTopicIds() const {}

// Edge operations
void TopicGraph::addEdge(uint32_t from, uint32_t to, Edge_Type type) {
    if (getTopic(from) == nullptr || getTopic(to) == nullptr)
        return;
    if (hasEdge(from, to, type))
        return;
    Edge edge;
    edge.targetId = to;
    edge.type = type;
    adjOutMap[from].push_back(edge);
    edge_count++;
}
bool TopicGraph::hasEdge(uint32_t from, uint32_t to) const {
    auto it = adjOutMap.find(from);
    if (it == adjOutMap.end())
        return false;

    for (const auto &edge : it->second) {
        if (edge.targetId == to) {
            return true;
        }
    }
    return false;
}
bool TopicGraph::hasEdge(uint32_t from, uint32_t to, Edge_Type type) const {
    auto it = adjOutMap.find(from);
    if (it == adjOutMap.end())
        return false;

    for (const auto &edge : it->second) {
        if (edge.targetId == to && edge.type == type) {
            return true;
        }
    }
    return false;
}
void TopicGraph::removeEdge(uint32_t from, uint32_t to) {
    auto it = adjOutMap[from].begin();
    while (it != adjOutMap[from].end()) {
        if (it->targetId == to) {
            break;
        }
        it++;
    }
    if (it != adjOutMap[from].end()) {
        adjOutMap[from].erase(it);
    }
}
std::vector<Edge> TopicGraph::getOutgoingEdges(uint32_t from) const {
    auto it = adjOutMap.find(from);
    if (it == adjOutMap.end())
        return {};
    return it->second;
}
std::vector<Edge> TopicGraph::getIncomingEdges(uint32_t to) const {}
std::vector<uint32_t> TopicGraph::getNeighbors(uint32_t from, Edge_Type type = Edge_Type::depends_on) const {}

// Utilities
size_t TopicGraph::numTopics() const {}
size_t TopicGraph::numEdges() const {}