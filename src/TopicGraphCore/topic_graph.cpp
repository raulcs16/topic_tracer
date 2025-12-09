#include "graph_keys.hpp"
#include "topic_graph.hpp"


TopicGraph::TopicGraph(ITopicStore &topicStore, IEdgeStore &edgeStore)
    : m_topics(topicStore), m_edges(edgeStore) {
    m_validators = {
        {EdgeType::ComposedOf,
         {
             [](auto f, auto t) { return isConcept(f); },
             [](auto f, auto t) { return isConcept(t); },
             [this](auto f, auto t) { return !hasParent(t->id, EdgeType::ComposedOf); },
             [this](auto f, auto t) {
                 return !makesCycle(f->id, t->id, EdgeType::ComposedOf);
             },
         }},

        {EdgeType::Example,
         {
             [](auto f, auto t) { return isConcept(f); },
             [](auto f, auto t) { return isConcrete(t); },
             [this](auto f, auto t) { return !hasParent(t->id, EdgeType::Example); },
             [this](auto f, auto t) {
                 return !makesCycle(f->id, t->id, EdgeType::Example);
             },
         }},

        {EdgeType::DependsOn,
         {
             [](auto f, auto t) { return isConcrete(f); },
             [](auto f, auto t) { return isConcrete(t); },
             [this](auto f, auto t) { return sameParent(f, t); },
         }},

        {EdgeType::AlternativeTo,
         {
             [](auto f, auto t) { return isConcrete(f); },
             [](auto f, auto t) { return isConcrete(t); },
             [this](auto f, auto t) { return sameParent(f, t); },
         }},
        {EdgeType::RelatedTo,
         {
             [](auto f, auto t) { return sameType(f, t); },
         }}};
}

//Topic API
const Topic *TopicGraph::addTopic(const std::string &name, TopicType topic_type) {
    if (getTopic(name) != nullptr)
        return nullptr;
    uint32_t id = nextId();
    auto topic = Topic{
        .id = id,
        .name = name,
        .type = topic_type,
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
    auto [parent, child] = normalizeJoin(a, b, type);
    std::string key = makeKey(parent->id, child->id);

    if (hasEdge(key))
        return nullptr;

    auto it = m_validators.find(type);
    if (it == m_validators.end())
        return nullptr;
    for (auto &rule : it->second) {
        if (!rule(parent, child))
            return nullptr;
    }

    auto edge = Edge(Edge{.key = key, .from = parent->id, .to = child->id, .type = type});
    m_edges.addEdge(edge);
    m_adjOutMap[parent->id].push_back(child->id);
    m_adjInMap[child->id].push_back(parent->id);
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

const Topic *TopicGraph::parent(uint32_t id) {
    auto it = m_adjInMap.find(id);
    if (it == m_adjInMap.end())
        return nullptr;

    for (uint32_t from : it->second) {
        auto edge = m_edges.getEdge(GraphKeys::key(from, id));
        if (!edge)
            continue;
        if (edge->type == EdgeType::ComposedOf || edge->type == EdgeType::Example) {
            return getTopic(edge->from);
        }
    }
    return nullptr;
}
std::vector<const Topic *> TopicGraph::childrenOf(uint32_t id) {
    auto outEdges = m_edges.getEdgesFrom(id);
    std::vector<const Topic *> children;
    for (const auto &e : outEdges) {
        auto topic = getTopic(e->to);
        if (topic) {
            children.push_back(topic);
        }
    }
    return children;
}
std::vector<uint32_t> TopicGraph::ancestorsOf(uint32_t id) {
    std::vector<uint32_t> ancestors;
    auto pt = parent(id);
    while (pt != nullptr) {
        ancestors.push_back(pt->id);
        pt = parent(pt->id);
    }
    return ancestors;
}
bool TopicGraph::hasParent(uint32_t node, EdgeType parentType) const {
    auto edges = m_edges.getEdgesTo(node);
    for (auto &e : edges) {
        if (e->type == parentType)
            return true;
    }
    return false;
}
bool TopicGraph::makesCycle(uint32_t from, uint32_t to, EdgeType type) {
    if (type != EdgeType::ComposedOf && type != EdgeType::Example)
        return false;
    std::unordered_set<uint32_t> visited;
    return dfsReachable(to, from, visited);
}
bool TopicGraph::dfsReachable(uint32_t start,
                              uint32_t target,
                              std::unordered_set<uint32_t> &visited) {
    if (start == target)
        return true;
    visited.insert(start);

    for (auto &edge : m_edges.getEdgesFrom(start)) {
        if (edge->type != EdgeType::ComposedOf && edge->type != EdgeType::Example)
            continue;

        uint32_t next = edge->to;
        if (!visited.count(next) && dfsReachable(next, target, visited))
            return true;
    }
    return false;
}
std::pair<const Topic *, const Topic *> TopicGraph::normalizeJoin(const Topic *a,
                                                                  const Topic *b,
                                                                  EdgeType type) {

    if (type != EdgeType::Example)
        return {a, b};
    if (a->type == TopicType::Concept && b->type == TopicType::Concrete)
        return {a, b};
    if (b->type == TopicType::Concept && a->type == TopicType::Concrete)
        return {b, a}; // swap
    return {a, b};
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