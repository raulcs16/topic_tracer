#include "topic_graph.hpp"


TopicGraph::TopicGraph() {
    m_edge_count = 0;
    m_id_ref = 1;
}


uint32_t TopicGraph::addTopic(const std::string &name, Topic_Type topic_type) {
    //TODO trim name
    if (getTopic(name) != nullptr)
        return 0;
    auto newTopic = std::make_shared<Topic>(m_id_ref++, name, topic_type);
    m_topicMap[newTopic->id()] = newTopic;
    m_adjOutMap[newTopic->id()] = {};
    return newTopic->id();
}
bool TopicGraph::renameTopic(uint32_t id, const std::string &new_name) {
    auto it = m_topicMap.find(id);
    if (it == m_topicMap.end())
        return false;
    auto exist = getTopic(new_name);
    if (exist != nullptr) {
        return false;
    }
    it->second->setName(new_name);
    return true;
}
void TopicGraph::deleteTopic(uint32_t id) {}
std::shared_ptr<const Topic> TopicGraph::getTopic(uint32_t id) const {
    auto it = m_topicMap.find(id);
    if (it != m_topicMap.end())
        return it->second;
    return nullptr;
}
std::shared_ptr<const Topic> TopicGraph::getTopic(const std::string &name) const {
    for (const auto &[id, topic] : m_topicMap) {
        if (topic->name() == name) {
            return topic;
        }
    }
    return nullptr;
}

void TopicGraph::addEdge(uint32_t from, uint32_t to, Edge_Type type) {
    if (getTopic(from) == nullptr || getTopic(to) == nullptr)
        return;
    if (hasEdge(from, to))
        return;
    Edge edge;
    edge.targetId = to;
    edge.type = type;
    m_adjOutMap[from].push_back(edge);
    m_edge_count++;
}
bool TopicGraph::hasEdge(uint32_t from, uint32_t to) const {
    auto it = m_adjOutMap.find(from);
    if (it == m_adjOutMap.end())
        return false;

    for (const auto &edge : it->second) {
        if (edge.targetId == to) {
            return true;
        }
    }
    return false;
}
void TopicGraph::removeEdge(uint32_t from, uint32_t to) {
    auto it = m_adjOutMap[from].begin();
    while (it != m_adjOutMap[from].end()) {
        if (it->targetId == to) {
            break;
        }
        it++;
    }
    if (it != m_adjOutMap[from].end()) {
        m_adjOutMap[from].erase(it);
    }
}
std::vector<Edge> TopicGraph::getOutEdges(uint32_t from) const {
    auto it = m_adjOutMap.find(from);
    if (it == m_adjOutMap.end())
        return {};
    return it->second;
}

// Utilities
size_t TopicGraph::numTopics() const { return m_topicMap.size(); }
size_t TopicGraph::numEdges() const { return m_edge_count; }

std::shared_ptr<const Topic> TopicGraph::operator[](size_t index) const {
    if (index >= m_topicMap.size()) {
        return nullptr;
    }
    auto it = m_topicMap.begin();
    std::advance(it, index);
    return it->second;
}
