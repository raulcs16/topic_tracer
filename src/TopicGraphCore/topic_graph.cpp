#include "graph_keys.hpp"
#include "topic_graph.hpp"
#include <iostream>


//Topic API
TopicGraph::~TopicGraph() { clear(); }
const Topic *TopicGraph::addTopic(const std::string &name) {
    if (getTopic(name) != nullptr)
        return nullptr;
    uint32_t id = nextId();
    auto topic = new Topic{.id = id, .name = name};
    m_topics[id] = topic;
    m_adjOutMap[id] = {};
    m_adjInMap[id] = {};
    notifyTopicAdded(*topic);
    return m_topics[id];
}
const Topic *TopicGraph::addTopic(uint32_t id, const std::string &name) {
    m_id_ref = id;
    if (getTopic(name) != nullptr) {
        return nullptr;
    }
    auto topic = new Topic{.id = id, .name = name};
    m_topics[id] = topic;
    m_adjOutMap[id] = {};
    m_adjInMap[id] = {};
    notifyTopicAdded(*topic);
    return m_topics[id];
}
bool TopicGraph::renameTopic(const std::string &name, const std::string &new_name) {
    auto topic = getTopic(name);
    if (topic == nullptr)
        return false;
    return renameTopic(topic->id, new_name);
}
bool TopicGraph::renameTopic(uint32_t id, const std::string &new_name) {
    auto it = m_topics.find(id);
    if (it == m_topics.end())
        return false;
    auto exist = getTopic(new_name);
    if (exist != nullptr) {
        return false;
    }
    it->second->name = new_name;
    notifyTopicRenamed(*it->second);
    return true;
}

bool TopicGraph::deleteTopic(const std::string &name) {
    auto topic = getTopic(name);
    if (topic == nullptr)
        return false;
    return deleteTopic(topic->id);
}

bool TopicGraph::deleteTopic(uint32_t id) {
    // Remove the topic
    auto topic = getTopic(id);
    if (!topic)
        return true;
    auto outEdges = getOutEdges(id);
    for (const auto edge : outEdges) {
        removeEdge(edge->from, edge->to);
    }
    auto inEdges = getInEdges(id);
    for (const auto edge : outEdges) {
        removeEdge(edge->from, edge->to);
    }
    notifyTopicRemoved(*topic);
    return m_topics.erase(id);
}

const Topic *TopicGraph::getTopic(uint32_t id) const {
    auto it = m_topics.find(id);
    return it == m_topics.end() ? nullptr : it->second;
}
const Topic *TopicGraph::getTopic(const std::string &name) const {
    auto found = std::find_if(m_topics.begin(), m_topics.end(), [name](auto &pair) {
        return pair.second->name == name;
    });
    return found == m_topics.end() ? nullptr : found->second;
}
std::vector<const Topic *> TopicGraph::topics() const {
    std::vector<const Topic *> result;
    result.reserve(m_topics.size());

    for (const auto &[id, topic] : m_topics) {
        result.push_back(topic);
    }
    return result;
}

/*
Enforce Rules such as making sure EdgeType matches TopicType(s) beign connected
*/
const Edge *TopicGraph::addEdge(Edge edge) {
    auto found = m_edges.find(edge.key);
    if (found != m_edges.end())
        return nullptr;
    m_edges[edge.key] = std::move(&edge);
    m_adjInMap[edge.to].push_back(edge.from);
    m_adjOutMap[edge.from].push_back(edge.to);
    notifyEdgeAdded(edge);
    return m_edges[edge.key];
}
const Edge *TopicGraph::addEdge(const Topic *a, const Topic *b, EdgeType type) {
    if (!a || !b)
        return nullptr;
    if (!m_topics.contains(a->id) || !m_topics.contains(b->id)) {
        return nullptr;
    }
    std::string key = makeKey(a->id, b->id);

    if (hasEdge(key))
        return nullptr;

    auto edge = new Edge(Edge{.key = key, .from = a->id, .to = b->id, .type = type});
    m_edges[key] = edge;
    m_adjOutMap[a->id].push_back(b->id);
    m_adjInMap[b->id].push_back(a->id);
    notifyEdgeAdded(*edge);
    return edge;
}
const Edge *TopicGraph::addEdge(uint32_t from, uint32_t to, EdgeType type) {
    auto pFrom = getTopic(from);
    auto pTo = getTopic(to);
    return addEdge(pFrom, pTo, type);
}
const Edge *TopicGraph::addEdge(const std::string &topicA,
                                const std::string &topicB,
                                EdgeType type) {
    auto ta = getTopic(topicA);
    auto tb = getTopic(topicB);
    return addEdge(ta, tb, type);
}
bool TopicGraph::hasEdge(const std::string &key) { return m_edges.contains(key); }
bool TopicGraph::removeEdge(uint32_t from, uint32_t to) {
    std::string key = makeKey(from, to);

    auto it = m_edges.find(key);
    if (it == m_edges.end())
        return true;

    auto &in = m_adjInMap[to];
    in.erase(std::remove(in.begin(), in.end(), from), in.end());

    auto &out = m_adjOutMap[from];
    out.erase(std::remove(out.begin(), out.end(), to), out.end());

    m_edges.erase(it);
    notifyEdgeRemoved(key);
    return true;
}
bool TopicGraph::removeEdge(const std::string &topicA, const std::string &topicB) {
    auto ta = getTopic(topicA);
    auto tb = getTopic(topicB);
    if (ta == nullptr || tb == nullptr)
        return false;
    return removeEdge(ta->id, tb->id);
}
const Edge *TopicGraph::getEdge(uint32_t from, uint32_t to) const {
    auto it = m_edges.find(GraphKeys::key(from, to));
    return it == m_edges.end() ? nullptr : it->second;
}
const Edge *TopicGraph::getEdge(const std::string &key) const {
    auto it = m_edges.find(key);
    return it == m_edges.end() ? nullptr : it->second;
}
std::vector<const Edge *> TopicGraph::edges() const {
    std::vector<const Edge *> result;
    result.reserve(m_edges.size());

    for (const auto &[_, edge] : m_edges) {
        result.push_back(edge);
    }
    return result;
}
std::vector<const Edge *> TopicGraph::getOutEdges(uint32_t from) const {
    auto it = m_adjOutMap.find(from);
    if (it == m_adjOutMap.end())
        return {};
    auto list = it->second;
    std::vector<const Edge *> result;
    result.reserve(list.size());
    for (auto to : list) {
        auto edge = getEdge(from, to);
        result.push_back(edge);
    }
    return result;
}
std::vector<const Edge *> TopicGraph::getInEdges(uint32_t to) const {
    auto it = m_adjInMap.find(to);
    if (it == m_adjOutMap.end())
        return {};
    auto list = it->second;
    std::vector<const Edge *> result;
    result.reserve(list.size());
    for (auto from : list) {
        auto edge = getEdge(from, to);
        result.push_back(edge);
    }
    return result;
}
std::vector<const Topic *> TopicGraph::parentsOf(uint32_t id) {
    auto it = m_adjInMap.find(id);
    if (it == m_adjInMap.end())
        return {};

    std::vector<const Topic *> parents;
    for (auto i = it->second.begin(); i != it->second.end(); i++) {
        auto t = m_topics.find((*i));
        if (t == m_topics.end())
            continue;
        parents.push_back(t->second);
    }
    return parents;
}
std::vector<const Topic *> TopicGraph::childrenOf(uint32_t id) {
    auto it = m_adjOutMap.find(id);
    if (it == m_adjOutMap.end())
        return {};

    std::vector<const Topic *> children;
    for (const auto id : it->second) {
        auto topic = getTopic(id);
        if (!topic)
            continue;
        children.push_back(topic);
    }
    return children;
}
uint32_t TopicGraph::nextId() { return m_id_ref++; }
std::string TopicGraph::makeKey(uint32_t from, uint32_t to) {
    return GraphKeys::key(from, to);
}

void TopicGraph::clear() {
    m_topics.clear();
    m_edges.clear();
    m_adjInMap.clear();
    m_adjOutMap.clear();
    m_id_ref = 1;
}

void TopicGraph::addObserver(ITopicGraphObserver *observer) {
    m_observers.push_back(observer);
}
void TopicGraph::removeObserver(ITopicGraphObserver *observer) {
    m_observers.erase(
        std::remove_if(m_observers.begin(),
                       m_observers.end(),
                       [observer](ITopicGraphObserver *it) { return it == observer; }));
}
void TopicGraph::notifyTopicAdded(const Topic &topic) {
    for (const auto obs : m_observers) {
        obs->onTopicAdded(topic);
    }
}
void TopicGraph::notifyTopicRemoved(const Topic &topic) {
    for (const auto obs : m_observers) {
        obs->onTopicRemoved(topic.id);
    }
}
void TopicGraph::notifyTopicRenamed(const Topic &topic) {
    for (const auto obs : m_observers) {
        obs->onTopicRenamed(topic);
    }
}
void TopicGraph::notifyEdgeAdded(const Edge &edge) {
    for (const auto obs : m_observers) {
        obs->onEdgeAdded(edge);
    }
}
void TopicGraph::notifyEdgeRemoved(const std::string &key) {
    for (const auto obs : m_observers) {
        obs->onEdgeRemoved(key);
    }
}
void TopicGraph::notifyClear() {
    for (const auto obs : m_observers) {
        obs->onClear();
    }
}