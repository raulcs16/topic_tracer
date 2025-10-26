#include "topic_graph.hpp"


TopicGraph::TopicGraph() : m_edge_count{0}, m_id_ref{1} {}

//Topic API

uint32_t TopicGraph::addTopic(const std::string &name, Topic_Type topic_type) {
    if (getTopic(name) != nullptr)
        return 0;
    uint32_t id = m_id_ref++;
    Topic topic{.id = id, .name = name, .type = topic_type, .covered = false};
    m_topicMap[id] = topic;
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
    it->second.name = new_name;
    return true;
}
bool TopicGraph::deleteTopic(uint32_t id) {
    m_topicMap.erase(id);
    m_adjOutMap.erase(id);

    for (auto &kv : m_adjOutMap) {
        kv.second.erase(std::remove_if(kv.second.begin(),
                                       kv.second.end(),
                                       [id](const Edge &e) { return e.to == id; }),
                        kv.second.end());
    }
    m_edge_count = 0;
    for (auto &kv : m_adjOutMap)
        m_edge_count += kv.second.size();
    return true;
}
std::shared_ptr<const Topic> TopicGraph::getTopic(uint32_t id) const {
    auto it = m_topicMap.find(id);
    if (it != m_topicMap.end())
        return std::make_shared<Topic>(it->second);
    return nullptr;
}
std::shared_ptr<const Topic> TopicGraph::getTopic(const std::string &name) const {
    for (const auto &[id, tnode] : m_topicMap) {
        if (tnode.name == name) {
            return std::make_shared<Topic>(tnode);
        }
    }
    return nullptr;
}
std::vector<std::shared_ptr<Topic>> TopicGraph::topics() const {
    std::vector<std::shared_ptr<Topic>> result;
    result.reserve(m_topicMap.size());
    for (auto &[_, topic] : m_topicMap)
        result.push_back(std::make_shared<Topic>(topic));
    return result;
}

//EDGE API
std::string TopicGraph::genKey(uint32_t from, uint32_t to) {
    std::string key = std::to_string(from) + "->" + std::to_string(to);
    return key;
}
std::shared_ptr<const Edge> TopicGraph::addEdge(uint32_t from,
                                                uint32_t to,
                                                Edge_Type type) {
    if (from == to)
        return nullptr;
    //aka key edge already exists
    if (hasEdge(from, to))
        return nullptr;
    //topics id's do not exist
    if (getTopic(from) == nullptr || getTopic(to) == nullptr)
        return nullptr;
    bool directed = true;
    switch (type) {
    case Edge_Type::ComposedOf: directed = true; break;
    case Edge_Type::AlternativeTo: directed = true; break;
    case Edge_Type::RelatedTo: directed = false; break;
    case Edge_Type::DependsOn: directed = true; break;
    case Edge_Type::Example: directed = true; break;
    default: directed = true; break;
    }
    std::string key = genKey(from, to);
    Edge edge{.key = key, .from = from, .to = to, .type = type, .directed = directed};
    m_adjOutMap[from].push_back(edge);
    m_keySet.insert(key);
    m_edge_count++;

    return std::make_shared<Edge>(edge);
}
std::shared_ptr<const Edge> TopicGraph::addEdge(const std::string &topicA,
                                                const std::string &topicB,
                                                Edge_Type type) {
    auto ta = getTopic(topicA);
    auto tb = getTopic(topicB);
    if (ta == nullptr || tb == nullptr)
        return nullptr;
    return addEdge(ta->id, tb->id, type);
}
bool TopicGraph::hasEdge(uint32_t from, uint32_t to) {
    return !(m_keySet.find(genKey(from, to)) == m_keySet.end());
}
bool TopicGraph::removeEdge(uint32_t from, uint32_t to) {
    if (!hasEdge(from, to)) {
        return false;
    }
    auto it = m_adjOutMap[from].begin();
    while (it != m_adjOutMap[from].end()) {
        if (it->to == to) {
            break;
        }
        it++;
    }
    if (it != m_adjOutMap[from].end()) {
        m_adjOutMap[from].erase(it);
        m_edge_count--;
    }
    return true;
}
bool TopicGraph::removeEdge(const std::string &topicA, const std::string &topicB) {
    auto ta = getTopic(topicA);
    auto tb = getTopic(topicB);
    if (ta == nullptr || tb == nullptr)
        return false;
    return removeEdge(ta->id, tb->id);
}
std::shared_ptr<const Edge> TopicGraph::getEdge(std::string key) {
    if (m_keySet.find(key) == m_keySet.end())
        return nullptr;
    uint32_t from = extractFrom(key);
    auto edges = m_adjOutMap[from];
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i].key == key) {
            return std::make_shared<Edge>(edges[i]);
        }
    }
    return nullptr;
}
std::shared_ptr<const Edge> TopicGraph::getEdge(uint32_t from, uint32_t to) {
    return getEdge(genKey(from, to));
}
std::vector<std::shared_ptr<Edge>> TopicGraph::edges() const {
    std::vector<std::shared_ptr<Edge>> result;
    for (auto &[_, edgeList] : m_adjOutMap)
        for (auto &edge : edgeList)
            result.push_back(std::make_shared<Edge>(edge));
    return result;
}
std::vector<Edge> TopicGraph::getOutEdges(uint32_t from) const {
    auto it = m_adjOutMap.find(from);
    if (it == m_adjOutMap.end())
        return {};
    return it->second;
}
std::vector<Edge> TopicGraph::getInEdges(uint32_t to) const {
    std::vector<Edge> inEdges;
    for (auto &[_, edgeList] : m_adjOutMap) {
        for (auto &edge : edgeList) {
            if (edge.to == to) {
                inEdges.push_back(edge);
            }
        }
    }

    return inEdges;
}