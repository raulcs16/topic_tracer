#include "graph_keys.hpp"
#include "topic_graph.hpp"


TopicGraph::TopicGraph() {}

//Topic API
uint32_t TopicGraph::addTopic(const std::string &name, TopicType topic_type) {
    if (getTopic(name) != nullptr)
        return 0;
    uint32_t id = nextId();
    auto ptr = std::make_shared<Topic>(
        Topic{.id = id, .name = name, .type = topic_type, .covered = false});
    m_topicMap[id] = ptr;
    m_adjOutMap[id] = {};
    m_adjInMap[id] = {};
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
    it->second.get()->name = new_name;
    return true;
}


bool TopicGraph::deleteTopic(uint32_t id) {
    // Remove the topic
    auto topicIt = m_topicMap.find(id);
    if (topicIt == m_topicMap.end())
        return false;
    m_topicMap.erase(topicIt);

    // Remove outgoing edges from adjacency map and edge map
    auto outIt = m_adjOutMap.find(id);
    if (outIt != m_adjOutMap.end()) {
        for (const auto &edge : outIt->second) {
            m_edgeMap.erase(edge->key);
            // Also remove from inMap of target
            auto inIt = m_adjInMap.find(edge->to);
            if (inIt != m_adjInMap.end()) {
                inIt->second.erase(std::remove_if(inIt->second.begin(),
                                                  inIt->second.end(),
                                                  [id](const std::shared_ptr<Edge> &e) {
                                                      return e->from == id;
                                                  }),
                                   inIt->second.end());
            }
        }
        m_adjOutMap.erase(outIt);
    }

    // Remove incoming edges from adjacency map and edge map
    auto inIt = m_adjInMap.find(id);
    if (inIt != m_adjInMap.end()) {
        for (const auto &edge : inIt->second) {
            m_edgeMap.erase(edge->key);
            // Also remove from outMap of source
            auto outSourceIt = m_adjOutMap.find(edge->from);
            if (outSourceIt != m_adjOutMap.end()) {
                outSourceIt->second.erase(
                    std::remove_if(
                        outSourceIt->second.begin(),
                        outSourceIt->second.end(),
                        [id](const std::shared_ptr<Edge> &e) { return e->to == id; }),
                    outSourceIt->second.end());
            }
        }
        m_adjInMap.erase(inIt);
    }

    return true;
}

std::shared_ptr<const Topic> TopicGraph::getTopic(uint32_t id) const {
    auto it = m_topicMap.find(id);
    if (it == m_topicMap.end())
        return nullptr;
    return it->second;
}
std::shared_ptr<const Topic> TopicGraph::getTopic(const std::string &name) const {
    for (const auto &[id, tnode] : m_topicMap) {
        if (tnode->name == name) {
            return tnode;
        }
    }
    return nullptr;
}
std::vector<std::shared_ptr<const Topic>> TopicGraph::topics() const {
    std::vector<std::shared_ptr<const Topic>> result;
    result.reserve(m_topicMap.size());
    for (auto &[_, topic] : m_topicMap)
        result.push_back(topic);
    return result;
}


std::shared_ptr<const Edge> TopicGraph::addEdge(uint32_t from,
                                                uint32_t to,
                                                EdgeType type) {
    if (from == to)
        return nullptr;
    //aka key edge already exists
    if (hasEdge(makeKey(from, to)))
        return nullptr;
    //topics id's do not exist
    if (getTopic(from) == nullptr || getTopic(to) == nullptr)
        return nullptr;
    bool directed = true;
    switch (type) {
    case EdgeType::ComposedOf: directed = true; break;
    case EdgeType::AlternativeTo: directed = true; break;
    case EdgeType::RelatedTo: directed = false; break;
    case EdgeType::DependsOn: directed = true; break;
    case EdgeType::Example: directed = true; break;
    default: directed = true; break;
    }
    std::string key = makeKey(from, to);
    auto edgePtr = std::make_shared<Edge>(
        Edge{.key = key, .from = from, .to = to, .type = type, .directed = directed});
    m_edgeMap[key] = edgePtr;
    m_adjOutMap[from].push_back(edgePtr);
    m_adjInMap[to].push_back(edgePtr);
    return edgePtr;
}
std::shared_ptr<const Edge> TopicGraph::addEdge(const std::string &topicA,
                                                const std::string &topicB,
                                                EdgeType type) {
    auto ta = getTopic(topicA);
    auto tb = getTopic(topicB);
    if (ta == nullptr || tb == nullptr)
        return nullptr;
    return addEdge(ta->id, tb->id, type);
}
bool TopicGraph::hasEdge(const std::string &key) { return m_edgeMap.count(key); }
bool TopicGraph::removeEdge(uint32_t from, uint32_t to) {
    std::string key = makeKey(from, to);
    auto it = m_edgeMap.find(key);
    if (it == m_edgeMap.end())
        return false;

    auto &outVec = m_adjOutMap[from];
    outVec.erase(std::remove_if(outVec.begin(),
                                outVec.end(),
                                [&key](const auto &e) { return e->key == key; }),
                 outVec.end());

    auto &inVec = m_adjInMap[to];
    inVec.erase(std::remove_if(inVec.begin(),
                               inVec.end(),
                               [&key](const auto &e) { return e->key == key; }),
                inVec.end());
    m_edgeMap.erase(it);
    return true;
}
bool TopicGraph::removeEdge(const std::string &topicA, const std::string &topicB) {
    auto ta = getTopic(topicA);
    auto tb = getTopic(topicB);
    if (ta == nullptr || tb == nullptr)
        return false;
    return removeEdge(ta->id, tb->id);
}
std::shared_ptr<const Edge> TopicGraph::getEdge(const std::string &key) const {
    auto it = m_edgeMap.find(key);
    return it != m_edgeMap.end() ? it->second : nullptr;
}
std::shared_ptr<const Edge> TopicGraph::getEdge(uint32_t from, uint32_t to) const {
    return getEdge(GraphKeys::key(from, to));
}
std::vector<std::shared_ptr<Edge>> TopicGraph::edges() const {
    std::vector<std::shared_ptr<Edge>> result;
    for (auto &[_, edge] : m_edgeMap) {
        result.push_back(edge);
    }
    return result;
}
std::vector<std::shared_ptr<Edge>> TopicGraph::getOutEdges(uint32_t from) const {
    auto it = m_adjOutMap.find(from);
    return it != m_adjOutMap.end() ? it->second : std::vector<std::shared_ptr<Edge>>{};
}
std::vector<std::shared_ptr<Edge>> TopicGraph::getInEdges(uint32_t to) const {
    auto it = m_adjInMap.find(to);
    return it != m_adjInMap.end() ? it->second : std::vector<std::shared_ptr<Edge>>{};
}

uint32_t TopicGraph::nextId() { return m_id_ref++; }
std::string TopicGraph::makeKey(uint32_t from, uint32_t to) {
    return GraphKeys::key(from, to);
}
