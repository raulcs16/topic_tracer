#include "graph_keys.hpp"
#include "topic_graph.hpp"


TopicGraph::TopicGraph() {
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
std::shared_ptr<const Topic> TopicGraph::addTopic(const std::string &name,
                                                  TopicType topic_type) {
    if (getTopic(name) != nullptr)
        return 0;
    uint32_t id = nextId();
    auto ptr = std::make_shared<Topic>(Topic{
        .id = id,
        .name = name,
        .type = topic_type,
    });
    m_topicMap[id] = ptr;
    m_adjOutMap[id] = {};
    m_adjInMap[id] = {};
    return ptr;
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

/*
Enforce Rules such as making sure EdgeType matches TopicType(s) beign connected
*/

std::shared_ptr<const Edge> TopicGraph::addEdge(const Topic *a,
                                                const Topic *b,
                                                EdgeType type) {
    if (!a || !b)
        return nullptr;
    if (!m_topicMap.contains(a->id) || !m_topicMap.contains(b->id)) {
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

    auto edgePtr = std::make_shared<Edge>(
        Edge{.key = key, .from = parent->id, .to = child->id, .type = type});
    m_edgeMap[key] = edgePtr;
    m_adjOutMap[parent->id].push_back(edgePtr);
    m_adjInMap[child->id].push_back(edgePtr);
    return edgePtr;
}
std::shared_ptr<const Edge> TopicGraph::addEdge(uint32_t from,
                                                uint32_t to,
                                                EdgeType type) {

    auto pFrom = getTopic(from);
    auto pTo = getTopic(to);
    return addEdge(pFrom.get(), pTo.get(), type);
}
std::shared_ptr<const Edge> TopicGraph::addEdge(const std::string &topicA,
                                                const std::string &topicB,
                                                EdgeType type) {
    auto ta = getTopic(topicA);
    auto tb = getTopic(topicB);
    return addEdge(ta.get(), tb.get(), type);
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

std::shared_ptr<const Topic> TopicGraph::parent(uint32_t id) {
    auto it = m_adjInMap.find(id);
    if (it == m_adjInMap.end())
        return nullptr;

    for (const auto &edge : it->second) {
        if (edge->type == EdgeType::ComposedOf || edge->type == EdgeType::Example) {
            return getTopic(edge->from);
        }
    }
    return nullptr;
}
std::vector<std::shared_ptr<const Topic>> TopicGraph::childrenOf(uint32_t id) {
    auto it = m_adjOutMap.find(id);
    if (it == m_adjOutMap.end()) {
        return {};
    }
    std::vector<std::shared_ptr<const Topic>> children;
    for (const auto &e : it->second) {
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
    auto it = m_adjInMap.find(node);
    if (it == m_adjInMap.end())
        return false;

    for (auto &e : it->second) {
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

    for (auto &edge : m_adjOutMap[start]) {
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
std::shared_ptr<const Topic> TopicGraph::addTopic(uint32_t id,
                                                  const std::string &name,
                                                  TopicType type) {
    if (getTopic(name) != nullptr || getTopic(id) != nullptr)
        return nullptr;

    m_id_ref = id + 1;
    auto ptr = std::make_shared<Topic>(Topic{
        .id = id,
        .name = name,
        .type = type,
    });

    m_topicMap[id] = ptr;
    m_adjOutMap[id] = {};
    m_adjInMap[id] = {};
    return ptr;
}
std::shared_ptr<const Edge> TopicGraph::addEdge(Edge edge) {
    if (!m_topicMap.contains(edge.from) || !m_topicMap.contains(edge.to)) {
        return nullptr;
    }

    if (hasEdge(edge.key))
        return nullptr;
    auto edgePtr = std::make_shared<Edge>(edge);
    m_edgeMap[edge.key] = edgePtr;
    m_adjOutMap[edge.from].push_back(edgePtr);
    m_adjInMap[edge.to].push_back(edgePtr);
    return edgePtr;
}

void TopicGraph::clear() {
    m_topicMap.clear();
    m_edgeMap.clear();
    m_adjInMap.clear();
    m_adjOutMap.clear();
    m_id_ref = 1;
}