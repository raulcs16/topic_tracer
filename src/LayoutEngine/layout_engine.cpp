#include "fermatspiral_strategy.hpp"
#include "graph_keys.hpp"
#include "layout_engine.hpp"


LayoutEngine::LayoutEngine()
    : m_ogdf{.attributes = ogdf::GraphAttributes(m_ogdf.graph,
                                                 ogdf::GraphAttributes::nodeGraphics |
                                                     ogdf::GraphAttributes::edgeGraphics |
                                                     ogdf::GraphAttributes::nodeLabel |
                                                     ogdf::GraphAttributes::edgeStyle)},
      m_strategy{new FermatSpiralStrategy()} {}
void LayoutEngine::setStrategy(std::unique_ptr<LayoutStrategy> strategy) {
    m_strategy = std::move(strategy);
}


void LayoutEngine::calculateLayout() {
    if (!m_strategy) {
        return;
    }
    m_strategy->apply(m_nodes, m_edges);
}

void LayoutEngine::clear() {
    m_ogdf.graph.clear();
    m_ogdf.keyToEdge.clear();
    m_ogdf.idToNode.clear();
    m_nodes.clear();
    m_edges.clear();
}
void LayoutEngine::addNode(uint32_t id) {
    auto node = m_ogdf.graph.newNode(id);
    m_ogdf.idToNode[id] = node;
    m_nodes.push_back(GraphNode{.id = id, .x = 0, .y = 0});
}
void LayoutEngine::removeNode(uint32_t id) {
    auto it = m_ogdf.idToNode.find(id);
    if (it == m_ogdf.idToNode.end()) {
        return;
    }
    m_ogdf.graph.delNode(it->second);
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
void LayoutEngine::addEdge(uint32_t from, uint32_t to) {
    auto fromNode = m_ogdf.idToNode.find(from);
    auto toNode = m_ogdf.idToNode.find(to);
    if (fromNode == m_ogdf.idToNode.end() || toNode == m_ogdf.idToNode.end()) {
        return;
    }
    auto edge = m_ogdf.graph.newEdge(fromNode->second, toNode->second);
    std::string k = GraphKeys::key(from, to);
    m_ogdf.keyToEdge[k] = edge;
    m_edges.push_back(GraphEdge{.key = k, .from = from, .to = to, .bends = {}});
}
void LayoutEngine::removeEdge(const std::string &k) {
    auto it = m_ogdf.keyToEdge.find(k);
    if (it == m_ogdf.keyToEdge.end()) {
        return;
    }
    m_ogdf.graph.delEdge(it->second);
    auto nit = m_edges.begin();
    while (nit != m_edges.end()) {
        if (nit->key == k)
            break;
        ++nit;
    }
    if (nit != m_edges.end()) {
        m_edges.erase(nit);
    }
}
