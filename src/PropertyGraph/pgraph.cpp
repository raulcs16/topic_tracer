#include "pgraph.hpp"

PGraph::PGraph(NodeRegPtr nodeReg, EdgeRegPtr edgeReg)
    : m_node_reg(nodeReg), m_edge_reg(edgeReg) {}

bool PGraph::addNode(type_id nodeType, const QString &name) {
    auto nodeDef = m_node_reg->get(nodeType);
    if (nodeDef == nullptr) {
        //provided node type was not found
        return false;
    }
    auto newNode = std::make_shared<Node>(Node{.id = ++m_node_id_ref,
                                               .name = name,
                                               .position = {.x = 0, .y = 0},
                                               .typeId = nodeType});
    m_nodes[newNode->id] = newNode;
    notify(&IPGObserver::onNodeAdded, newNode->id);
    return true;
}
std::shared_ptr<const Node> PGraph::getNode(const QString &name) {
    auto found = std::find_if(m_nodes.begin(), m_nodes.end(), [&name](const auto &pair) {
        return pair.second->name == name;
    });
    if (found == m_nodes.end()) {
        return nullptr;
    }
    return static_cast<std::shared_ptr<const Node>>(found->second);
}
bool PGraph::addEdge(type_id edgeType, node_id from, node_id to) {
    auto edgeDef = m_edge_reg->get(edgeType);
    if (edgeDef == nullptr) {
        //edge type not found
        return false;
    }
    if (from == to && !edgeDef->allowSelfLoops) {
        //cant allow self loops
        return false;
    }
    auto fromNode = m_nodes.find(from);
    auto toNode = m_nodes.find(to);
    if (fromNode == m_nodes.end() || toNode == m_nodes.end()) {
        //one or more nodes missing
        return false;
    }
    auto fromType = m_node_reg->get(fromNode->second->typeId);
    auto toType = m_node_reg->get(toNode->second->typeId);
    if (!fromType || !toType) {
        //one or more types not found
        return false;
    }
    if (!edgeDef->allowed_dir.count(
            AllowedDir{.from = fromType->typeId, .to = toType->typeId})) {
        //not an allowed direction
        return false;
    }
    //add edge
    auto edge = std::make_shared<Edge>(Edge{.id = ++m_edge_id_ref,
                                            .typeId = edgeType,
                                            .from = from,
                                            .to = to,
                                            .line = tt::Line()});
    m_edges[edge->id] = edge;

    notify(&IPGObserver::onEdgeAdded, edge->id);
    return true;
}
void PGraph::addObserver(ObserPtr observer) {
    auto found = std::find(m_observers.begin(), m_observers.end(), observer);
    if (found == m_observers.end()) {
        m_observers.push_back(observer);
    }
}

template <typename Func, typename... Args>
void PGraph::notify(Func memberFunc, Args &&...args) {
    for (auto &obs : m_observers) {
        (obs.get()->*memberFunc)(std::forward<Args>(args)...);
    }
}