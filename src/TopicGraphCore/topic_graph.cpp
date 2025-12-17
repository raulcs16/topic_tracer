#include "graph_keys.hpp"
#include "topic_graph.hpp"


TopicGraph::TopicGraph(ITopicStore &topicStore, IEdgeStore &edgeStore)
    : m_topics(topicStore), m_edges(edgeStore) {}

//Topic API
const Topic *TopicGraph::addTopic(const std::string &name) {
    if (getTopic(name) != nullptr)
        return nullptr;
    uint32_t id = nextId();
    auto topic = Topic{
        .id = id,
        .name = name,
    };
    if (!m_topics.addTopic(topic))
        return nullptr;
    m_adjOutMap[id] = {};
    m_adjInMap[id] = {};
    return m_topics.getTopic(topic.id);
}
bool TopicGraph::renameTopic(uint32_t id, const std::string &new_name) {
    auto topic = m_topics.getTopic(id);
    if (!topic)
        return false;
    auto exist = m_topics.findTopic(new_name);
    if (!exist) {
        return false;
    }
    return m_topics.renameTopic(topic->id, new_name);
}


bool TopicGraph::deleteTopic(uint32_t id) {
    // Remove the topic
    auto topic = m_topics.getTopic(id);
    if (!topic)
        return false;
    auto outIt = m_adjOutMap.find(id);
    if (outIt != m_adjOutMap.end()) {
        for (auto to : outIt->second) {
            m_edges.removeEdge(GraphKeys::key(topic->id, to));
            auto inIt = m_adjInMap.find(to);
            if (inIt != m_adjInMap.end()) {
                inIt->second.erase(
                    std::remove_if(inIt->second.begin(),
                                   inIt->second.end(),
                                   [id](uint32_t from) { return from == id; }),
                    inIt->second.end());
            }
        }
        m_adjOutMap.erase(outIt);
    }

    // Remove incoming edges from adjacency map and edge map
    auto inIt = m_adjInMap.find(id);
    if (inIt != m_adjInMap.end()) {
        for (auto from : inIt->second) {
            m_edges.removeEdge(GraphKeys::key(from, id));
            // Also remove from outMap of source
            auto outSourceIt = m_adjOutMap.find(from);
            if (outSourceIt != m_adjOutMap.end()) {
                outSourceIt->second.erase(
                    std::remove_if(outSourceIt->second.begin(),
                                   outSourceIt->second.end(),
                                   [id](uint32_t to) { return to == id; }),
                    outSourceIt->second.end());
            }
        }
        m_adjInMap.erase(inIt);
    }
    return m_topics.removeTopic(id);
}

const Topic *TopicGraph::getTopic(uint32_t id) const { return m_topics.getTopic(id); }
const Topic *TopicGraph::getTopic(const std::string &name) const {
    return m_topics.findTopic(name);
}
std::vector<const Topic *> TopicGraph::topics() const { return m_topics.getAllTopics(); }

/*
Enforce Rules such as making sure EdgeType matches TopicType(s) beign connected
*/

const Edge *TopicGraph::addEdge(const Topic *a, const Topic *b, EdgeType type) {
    if (!a || !b)
        return nullptr;
    if (!m_topics.contains(a->id) || !m_topics.contains(b->id)) {
        return nullptr;
    }
    std::string key = makeKey(a->id, b->id);

    if (hasEdge(key))
        return nullptr;

    auto edge = Edge(Edge{.key = key, .from = a->id, .to = b->id, .type = type});
    m_edges.addEdge(edge);
    m_adjOutMap[a->id].push_back(b->id);
    m_adjInMap[b->id].push_back(a->id);
    return m_edges.getEdge(edge.key);
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
    auto success = m_edges.removeEdge(key);
    if (!success)
        return false;

    auto &outVec = m_adjOutMap[from];
    outVec.erase(std::remove_if(outVec.begin(),
                                outVec.end(),
                                [&to](uint32_t key) { return key == to; }),
                 outVec.end());

    auto &inVec = m_adjInMap[to];
    inVec.erase(std::remove_if(inVec.begin(),
                               inVec.end(),
                               [&from](uint32_t key) { return from == key; }),
                inVec.end());
    return success;
}
bool TopicGraph::removeEdge(const std::string &topicA, const std::string &topicB) {
    auto ta = getTopic(topicA);
    auto tb = getTopic(topicB);
    if (ta == nullptr || tb == nullptr)
        return false;
    return removeEdge(ta->id, tb->id);
}
const Edge *TopicGraph::getEdge(uint32_t from, uint32_t to) const {
    return m_edges.getEdge(GraphKeys::key(from, to));
}
std::vector<const Edge *> TopicGraph::edges() const { return m_edges.getAllEdges(); }
std::vector<const Edge *> TopicGraph::getOutEdges(uint32_t from) const {
    return m_edges.getEdgesFrom(from);
}
std::vector<const Edge *> TopicGraph::getInEdges(uint32_t to) const {
    return m_edges.getEdgesTo(to);
}

std::vector<const Topic *> TopicGraph::parentsOf(uint32_t id) {
    auto it = m_adjInMap.find(id);
    if (it == m_adjInMap.end())
        return {};

    std::vector<const Topic *> parents;
    for (auto i = it->second.begin(); i != it->second.end(); i++) {
        auto topic = m_topics.getTopic((*i));
        if (!topic)
            continue;
        parents.push_back(topic);
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