#include "graph_keys.hpp"
#include "ogdf_cluster.hpp"

OGDFCluster::OGDFCluster(uint32_t id, std::shared_ptr<OGDFStrategy> strategy)
    : m_id(id), m_strategy{strategy} {
    m_ogdf = std::make_shared<OGDFContext>();
    m_ogdf->attributes = ogdf::GraphAttributes(m_ogdf->graph);
    m_strategy->setContext(m_ogdf);
}
void OGDFCluster::addNode(uint32_t id) {
    auto node = m_ogdf->graph.newNode(id);
    m_ogdf->idToNode[id] = node;
    m_nodes.emplace_back(id, 0, 0);
}
GraphNode *OGDFCluster::getNode(uint32_t id) const {
    for (size_t i = 0; i < m_nodes.size(); ++i) {
        if (m_nodes[i].id == id) {
            return const_cast<GraphNode *>(&m_nodes[i]);
        }
    }
    return nullptr;
}
void OGDFCluster::removeNode(uint32_t id) {
    auto it = m_ogdf->idToNode.find(id);
    if (it == m_ogdf->idToNode.end()) {
        return;
    }
    m_ogdf->graph.delNode(it->second);
    auto nit = m_nodes.begin();
    while (nit != m_nodes.end()) {
        if (nit->id == id)
            break;
        ++nit;
    }
    if (nit != m_nodes.end()) {
        m_nodes.erase(nit);
    }
}
void OGDFCluster::addEdge(uint32_t from, uint32_t to) {
    auto fromNode = m_ogdf->idToNode.find(from);
    auto toNode = m_ogdf->idToNode.find(to);
    if (fromNode == m_ogdf->idToNode.end() || toNode == m_ogdf->idToNode.end()) {
        return;
    }
    auto edge = m_ogdf->graph.newEdge(fromNode->second, toNode->second);
    std::string k = GraphKeys::key(from, to);
    m_ogdf->keyToEdge[k] = edge;
    m_edges.push_back(GraphEdge{.key = k, .from = from, .to = to, .bends = {}});
}
void OGDFCluster::removeEdge(uint32_t from, uint32_t to) {
    auto key = GraphKeys::key(from, to);
    auto it = m_ogdf->keyToEdge.find(key);
    if (it == m_ogdf->keyToEdge.end()) {
        return;
    }
    m_ogdf->graph.delEdge(it->second);
    auto nit = m_edges.begin();
    while (nit != m_edges.end()) {
        if (nit->key == key)
            break;
        ++nit;
    }
    if (nit != m_edges.end()) {
        m_edges.erase(nit);
    }
}
void OGDFCluster::clear() {
    m_ogdf->graph.clear();
    m_ogdf->idToNode.clear();
    m_nodes.clear();
    m_edges.clear();
}
void OGDFCluster::apply() {
    if (m_strategy)
        m_strategy->apply(m_nodes, m_edges, m_bb);
}
BoundingBox OGDFCluster::boundingBox() const { return m_bb; }
void OGDFCluster::setStrategy(std::shared_ptr<LayoutStrategy> s) {
    auto ogdfStrat = std::dynamic_pointer_cast<OGDFStrategy>(s);
    if (!ogdfStrat) {
        throw std::invalid_argument("OGDFCluster only accepts OGDFStrategy");
    }
    m_strategy = ogdfStrat;
}