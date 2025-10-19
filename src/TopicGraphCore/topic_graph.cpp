#include "topic_graph.hpp"


TopicGraph::TopicGraph() : m_edge_count{0}, m_id_ref{1} {
    auto a = addTopic("DesignPatterns");
    auto b = addTopic("StructualPatterns");
    auto c = addTopic("CreationalPatterns");
    auto d = addTopic("BehaviouralPatterns");
    addEdge(a, b, Edge_Type::ComposedOf);
    addEdge(a, c, Edge_Type::ComposedOf);
    addEdge(a, d, Edge_Type::ComposedOf);
}


uint32_t TopicGraph::addTopic(const std::string &name, Topic_Type topic_type) {
    if (getTopic(name) != nullptr)
        return 0;
    uint32_t id = m_id_ref++;
    Topic topic{.id = id, .name = name, .type = topic_type, .covered = false};
    GraphNode geometry{.id = id, .label = name, .x = 0, .y = 0};
    m_topicMap[id] = TopicNode{.topic = topic, .geometry = geometry};
    m_adjOutMap[id] = {};
    return id;
}
bool TopicGraph::renameTopic(uint32_t id, const std::string &new_name) {
    auto it = m_topicMap.find(id);
    if (it == m_topicMap.end())
        return false;
    auto exist = getTopic(new_name);
    if (exist != nullptr) {
        return false;
    }
    it->second.topic.name = new_name;
    it->second.geometry.label = new_name;
    return true;
}
void TopicGraph::deleteTopic(uint32_t id) {
    m_topicMap.erase(id);
    m_adjOutMap.erase(id);
    //loop to delete refefences to it

    for (auto &kv : m_adjOutMap) {
        kv.second.erase(std::remove_if(kv.second.begin(),
                                       kv.second.end(),
                                       [id](const TopicEdge &e) { return e.to == id; }),
                        kv.second.end());
    }
    m_edge_count = 0;
    for (auto &kv : m_adjOutMap)
        m_edge_count += kv.second.size();
}
std::shared_ptr<const TopicNode> TopicGraph::getTopic(uint32_t id) const {
    auto it = m_topicMap.find(id);
    if (it != m_topicMap.end())
        return std::make_shared<TopicNode>(it->second);
    return nullptr;
}
std::shared_ptr<const TopicNode> TopicGraph::getTopic(const std::string &name) const {
    for (const auto &[id, tnode] : m_topicMap) {
        if (tnode.topic.name == name) {
            return std::make_shared<TopicNode>(tnode);
        }
    }
    return nullptr;
}

void TopicGraph::addEdge(uint32_t from, uint32_t to, Edge_Type type) {
    if (getTopic(from) == nullptr || getTopic(to) == nullptr)
        return;
    if (hasEdge(from, to))
        return;
    TopicEdge edge{.from = from,
                   .to = to,
                   .relation = type,
                   .geometry = {.bends = {}, .from = from, .to = to}};

    m_adjOutMap[from].push_back(edge);
    m_edge_count++;
}
bool TopicGraph::hasEdge(uint32_t from, uint32_t to) const {
    auto it = m_adjOutMap.find(from);
    if (it == m_adjOutMap.end())
        return false;

    for (const auto &edge : it->second) {
        if (edge.to == to) {
            return true;
        }
    }
    return false;
}
void TopicGraph::removeEdge(uint32_t from, uint32_t to) {
    auto it = m_adjOutMap[from].begin();
    while (it != m_adjOutMap[from].end()) {
        if (it->to == to) {
            break;
        }
        it++;
    }
    if (it != m_adjOutMap[from].end()) {
        m_adjOutMap[from].erase(it);
    }
}
std::vector<TopicEdge> TopicGraph::getOutEdges(uint32_t from) const {
    auto it = m_adjOutMap.find(from);
    if (it == m_adjOutMap.end())
        return {};
    return it->second;
}

// Utilities
size_t TopicGraph::numTopics() const { return m_topicMap.size(); }
size_t TopicGraph::numEdges() const { return m_edge_count; }

std::shared_ptr<const TopicNode> TopicGraph::operator[](size_t index) const {
    if (index >= m_topicMap.size()) {
        return nullptr;
    }
    auto it = m_topicMap.begin();
    std::advance(it, index);
    return std::make_shared<TopicNode>(it->second);
}
std::vector<std::reference_wrapper<TopicNode>> TopicGraph::topics() {
    std::vector<std::reference_wrapper<TopicNode>> result;
    result.reserve(m_topicMap.size());
    for (auto &[_, node] : m_topicMap)
        result.push_back(node);
    return result;
}

std::vector<std::reference_wrapper<TopicEdge>> TopicGraph::edges() {
    std::vector<std::reference_wrapper<TopicEdge>> result;
    for (auto &[_, edgeList] : m_adjOutMap)
        for (auto &edge : edgeList)
            result.push_back(edge);
    return result;
}

std::vector<GraphNode> TopicGraph::graphNodes() {
    std::vector<GraphNode> result;
    result.reserve(m_topicMap.size());
    for (auto &[_, node] : m_topicMap)
        result.push_back(node.geometry);
    return result;
}
std::vector<GraphEdge> TopicGraph::graphEdges() {
    std::vector<GraphEdge> result;
    for (auto &[_, edgeList] : m_adjOutMap)
        for (auto &edge : edgeList)
            result.push_back(edge.geometry);
    return result;
}