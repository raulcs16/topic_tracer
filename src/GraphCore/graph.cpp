#include "graph.hpp"
#include "graph_keys.hpp"
#include <iostream>


//Node API
Graph::~Graph() {
    clear();
    m_observers.clear();
}
bool Graph::addNode(const std::string &label) {
    if (getNode(label) != nullptr)
        return false;
    uint32_t id = nextId();
    // std::cout << "tg:addTopic:" << label << ",id:" << id << std::endl;
    auto node = Node{.id = id, .label = label};
    m_nodes[id] = node;
    m_adjOutMap[id] = {};
    m_adjInMap[id] = {};
    notify(&IGraphObserver::onNodeAdded, node);
    return true;
}
bool Graph::addNode(uint32_t id, const std::string &label) {
    m_id_ref = std::max(m_id_ref, id);
    if (getNode(label) != nullptr) {
        return false;
    }
    auto node = Node{.id = id, .label = label};
    m_nodes[id] = node;
    m_adjOutMap[id] = {};
    m_adjInMap[id] = {};
    notify(&IGraphObserver::onNodeAdded, node);
    return true;
}
bool Graph::renameNode(const std::string &label, const std::string &new_name) {
    auto node = getNode(label);
    if (node == nullptr)
        return false;
    return renameNode(node->id, new_name);
}
bool Graph::renameNode(uint32_t id, const std::string &new_name) {
    auto it = m_nodes.find(id);
    if (it == m_nodes.end())
        return false;
    auto exist = getNode(new_name);
    if (exist != nullptr) {
        return false;
    }
    it->second.label = new_name;
    notify(&IGraphObserver::onNodeRenamed, (it->second));
    return true;
}

bool Graph::deleteNode(const std::string &label) {
    auto node = getNode(label);
    if (node == nullptr)
        return false;
    return deleteNode(node->id);
}

bool Graph::deleteNode(uint32_t id) {
    // Remove the node
    auto node = getNode(id);
    if (!node)
        return true;
    auto outEdges = getOutEdges(id);
    for (const auto edge : outEdges) {
        removeEdge(edge->from, edge->to);
    }
    auto inEdges = getInEdges(id);
    for (const auto edge : outEdges) {
        removeEdge(edge->from, edge->to);
    }
    notify(&IGraphObserver::onNodeRemoved, id);
    return m_nodes.erase(id);
}

const Node *Graph::getNode(uint32_t id) const {
    auto it = m_nodes.find(id);
    return it == m_nodes.end() ? nullptr : &it->second;
}
const Node *Graph::getNode(const std::string &label) const {
    auto found = std::find_if(m_nodes.begin(), m_nodes.end(), [label](auto &pair) {
        return pair.second.label == label;
    });
    return found == m_nodes.end() ? nullptr : &found->second;
}
std::vector<const Node *> Graph::nodes() const {
    std::vector<const Node *> result;
    result.reserve(m_nodes.size());

    for (const auto &[id, node] : m_nodes) {
        result.push_back(&node);
    }
    return result;
}

bool Graph::addEdge(Edge edge) {
    auto [it, inserted] = m_edges.try_emplace(edge.key, std::move(edge));
    if (!inserted) {
        return false;
    }
    const Edge &storedEdge = it->second;

    m_adjInMap[storedEdge.to].push_back(storedEdge.from);
    m_adjOutMap[storedEdge.from].push_back(storedEdge.to);

    notify(&IGraphObserver::onEdgeAdded, storedEdge);
    return true;
}

bool Graph::addEdge(const Node *a, const Node *b, EdgeType type) {
    if (!a || !b)
        return false;
    if (!m_nodes.contains(a->id) || !m_nodes.contains(b->id)) {
        return false;
    }
    std::string key = makeKey(a->id, b->id);

    if (hasEdge(key))
        return false;
    auto edge = Edge(Edge{.key = key, .from = a->id, .to = b->id, .type = type});
    m_edges[key] = edge;
    m_adjOutMap[a->id].push_back(b->id);
    m_adjInMap[b->id].push_back(a->id);
    notify(&IGraphObserver::onEdgeAdded, edge);
    return true;
}
bool Graph::addEdge(uint32_t from, uint32_t to, EdgeType type) {
    auto pFrom = getNode(from);
    auto pTo = getNode(to);
    return addEdge(pFrom, pTo, type);
}
bool Graph::addEdge(const std::string &topicA, const std::string &topicB, EdgeType type) {
    auto ta = getNode(topicA);
    auto tb = getNode(topicB);
    return addEdge(ta, tb, type);
}
bool Graph::hasEdge(const std::string &key) { return m_edges.contains(key); }
bool Graph::removeEdge(uint32_t from, uint32_t to) {
    std::string key = makeKey(from, to);

    auto it = m_edges.find(key);
    if (it == m_edges.end())
        return true;

    auto &in = m_adjInMap[to];
    in.erase(std::remove(in.begin(), in.end(), from), in.end());

    auto &out = m_adjOutMap[from];
    out.erase(std::remove(out.begin(), out.end(), to), out.end());

    m_edges.erase(it);
    notify(&IGraphObserver::onEdgeRemoved, key);
    return true;
}
bool Graph::removeEdge(const std::string &topicA, const std::string &topicB) {
    auto ta = getNode(topicA);
    auto tb = getNode(topicB);
    if (ta == nullptr || tb == nullptr)
        return false;
    return removeEdge(ta->id, tb->id);
}
const Edge *Graph::getEdge(uint32_t from, uint32_t to) const {
    auto it = m_edges.find(GraphKeys::key(from, to));
    return it == m_edges.end() ? nullptr : &it->second;
}
const Edge *Graph::getEdge(const std::string &key) const {
    auto it = m_edges.find(key);
    return it == m_edges.end() ? nullptr : &it->second;
}
std::vector<const Edge *> Graph::edges() const {
    std::vector<const Edge *> result;
    result.reserve(m_edges.size());

    for (const auto &[_, edge] : m_edges) {
        result.push_back(&edge);
    }
    return result;
}
std::vector<const Edge *> Graph::getOutEdges(uint32_t from) const {
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
std::vector<const Edge *> Graph::getInEdges(uint32_t to) const {
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
std::vector<const Node *> Graph::parentsOf(uint32_t id) {
    auto it = m_adjInMap.find(id);
    if (it == m_adjInMap.end())
        return {};

    std::vector<const Node *> parents;
    for (auto i = it->second.begin(); i != it->second.end(); i++) {
        auto t = m_nodes.find((*i));
        if (t == m_nodes.end())
            continue;
        parents.push_back(&t->second);
    }
    return parents;
}
std::vector<const Node *> Graph::childrenOf(uint32_t id) {
    auto it = m_adjOutMap.find(id);
    if (it == m_adjOutMap.end())
        return {};

    std::vector<const Node *> children;
    for (const auto id : it->second) {
        auto node = getNode(id);
        if (!node)
            continue;
        children.push_back(node);
    }
    return children;
}
uint32_t Graph::nextId() { return ++m_id_ref; }
std::string Graph::makeKey(uint32_t from, uint32_t to) {
    return GraphKeys::key(from, to);
}

void Graph::clear() {
    m_nodes.clear();
    m_edges.clear();
    m_adjInMap.clear();
    m_adjOutMap.clear();
    m_id_ref = 1;
    notify(&IGraphObserver::onClear);
}

void Graph::addObserver(IGraphObserver *observer) { m_observers.push_back(observer); }
void Graph::removeObserver(IGraphObserver *observer) {
    m_observers.erase(
        std::remove_if(m_observers.begin(),
                       m_observers.end(),
                       [observer](IGraphObserver *it) { return it == observer; }));
}
template <typename Func, typename... Args>
void Graph::notify(Func memberFunc, Args &&...args) {
    if (m_isLoading)
        return;
    for (auto *obs : m_observers) {
        (obs->*memberFunc)(std::forward<Args>(args)...);
    }
}

void Graph::beginBatchLoad() { m_isLoading = true; }
void Graph::endBatchLoad() {
    m_isLoading = false;
    GraphBlueprint blueprint = buildGraphBlueprint();

    notify(&IGraphObserver::onGraphBluePrint, blueprint);
}

GraphBlueprint Graph::buildGraphBlueprint() {
    //dfs
    GraphBlueprint blueprint;
    std::unordered_set<uint32_t> visited;
    for (auto const &[id, node] : m_nodes) {
        //already accounted for
        if (visited.contains(id))
            continue;
        //check if has edges
        if (m_adjInMap[id].empty() && m_adjOutMap[id].empty()) {
            blueprint.isoNodes.push_back(node);
            visited.insert(id);
            continue;
        }
        //has edges aka part of a cluster
        SemanticCluster cluster;
        std::vector<uint32_t> stack = {id};
        std::unordered_set<std::string> addedEdges;
        visited.insert(id);
        //dfs
        while (!stack.empty()) {
            uint32_t curr = stack.back();
            stack.pop_back();
            auto node = getNode(curr);
            if (node == nullptr)
                continue;
            cluster.nodes.push_back(*node);

            // Explore Outbound neighbors
            for (const Edge *edge : getOutEdges(curr)) {
                if (!addedEdges.contains(edge->key)) {
                    cluster.edges.push_back(*edge);
                    addedEdges.insert(edge->key);
                }
                if (!visited.contains(edge->to)) {
                    visited.insert(edge->to);
                    stack.push_back(edge->to);
                }
            }

            // Explore Inbound neighbors (treating as undirected)
            for (const Edge *edge : getInEdges(curr)) {
                if (!addedEdges.contains(edge->key)) {
                    cluster.edges.push_back(*edge);
                    addedEdges.insert(edge->key);
                }
                if (!visited.contains(edge->from)) {
                    visited.insert(edge->from);
                    stack.push_back(edge->from);
                }
            }
        }
        blueprint.clusters.push_back(std::move(cluster));
    }
    return blueprint;
}