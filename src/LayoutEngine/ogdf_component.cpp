#include "graph_keys.hpp"
#include "ogdf_component.hpp"

OGDFComponent::OGDFComponent(std::shared_ptr<OGDFStrategy> strategy)
    : m_strategy{strategy} {
    m_ogdf = std::make_shared<OGDFContext>();
    m_ogdf->attributes = ogdf::GraphAttributes(m_ogdf->graph);
    m_strategy->setContext(m_ogdf);
}
GraphNode &OGDFComponent::addNode(uint32_t id) {
    appendNode(id);
    apply();
    return m_nodes.back();
}
void OGDFComponent::appendNode(uint32_t id) {
    auto node = m_ogdf->graph.newNode(id);
    m_ogdf->idToNode[id] = node;
    m_nodes.emplace_back(id, 0, 0);
}
void OGDFComponent::removeNode(uint32_t id) {
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
void OGDFComponent::addEdge(uint32_t from, uint32_t to) {
    auto fromNode = m_ogdf->idToNode.find(from);
    auto toNode = m_ogdf->idToNode.find(to);
    if (fromNode == m_ogdf->idToNode.end() || toNode == m_ogdf->idToNode.end()) {
        return;
    }
    auto edge = m_ogdf->graph.newEdge(fromNode->second, toNode->second);
    std::string k = GraphKeys::key(from, to);
    m_ogdf->keyToEdge[k] = edge;
    m_edges.push_back(GraphEdge{.key = k, .from = from, .to = to, .bends = {}});
    apply();
}
void OGDFComponent::removeEdge(uint32_t from, uint32_t to) {
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
void OGDFComponent::clear() {
    m_ogdf->graph.clear();
    m_ogdf->idToNode.clear();
    m_nodes.clear();
    m_edges.clear();
}
void OGDFComponent::apply() {
    if (m_strategy)
        m_strategy->apply(m_nodes, m_edges);
}
BoundingBox OGDFComponent::boundingBox() const { return m_bb; }
void OGDFComponent::setStrategy(std::shared_ptr<LayoutStrategy> s) {
    auto ogdfStrat = std::dynamic_pointer_cast<OGDFStrategy>(s);
    if (!ogdfStrat) {
        throw std::invalid_argument("OGDFComponent only accepts OGDFStrategy");
    }
    m_strategy = ogdfStrat;
}