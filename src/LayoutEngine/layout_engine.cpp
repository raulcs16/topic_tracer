#include "layout_engine.hpp"


LayoutEngine::LayoutEngine()
    : m_graph(), m_attributes(m_graph,
                              ogdf::GraphAttributes::nodeGraphics |
                                  ogdf::GraphAttributes::edgeGraphics |
                                  ogdf::GraphAttributes::nodeLabel |
                                  ogdf::GraphAttributes::edgeStyle),
      m_strategy{nullptr} {}
void LayoutEngine::setStrategy(std::unique_ptr<LayoutStrategy> strategy) {
    m_strategy = std::move(strategy);
}


void LayoutEngine::calculateLayout() {
    if (m_edges.size() == 0) {
        assignFermatSpiralPositions();
        return;
    }
    try {
        if (!m_strategy) {
            return;
        }
        m_strategy->apply(m_attributes);
        m_attributes.addNodeCenter2Bends();
        updateNodesFromAttributes();

    } catch (ogdf::AlgorithmFailureException &e) {
        std::cerr << "Algorithmn Failure: " << (int)(e.exceptionCode()) << "\n";
    } catch (...) {
        std::cerr << "unknown error\n";
    }
}
void LayoutEngine::updateNodesFromAttributes() {
    for (auto &node : m_nodes) {
        auto it = m_idToNode.find(node.id);
        if (it == m_idToNode.end())
            continue;
        node.x = m_attributes.x(it->second);
        node.y = m_attributes.y(it->second);
    }
    for (auto &edge : m_edges) {
        auto src = m_idToNode.find(edge.from);
        auto target = m_idToNode.find(edge.to);
        if (src == m_idToNode.end() || target == m_idToNode.end())
            continue;

        edge.source_x = m_attributes.x(src->second);
        edge.source_y = m_attributes.y(src->second);
        edge.target_x = m_attributes.x(target->second);
        edge.target_y = m_attributes.y(target->second);
        auto e = m_idToEdge.find(edge.key);
        if (e == m_idToEdge.end())
            continue;
        edge.bends.clear();
        for (ogdf::DPoint &p : m_attributes.bends(e->second)) {
            edge.bends.push_back(p);
        }
    }
}
void LayoutEngine::clear() {
    m_graph.clear();
    m_idToEdge.clear();
    m_idToNode.clear();
    m_nodes.clear();
    m_edges.clear();
}
void LayoutEngine::addNode(uint32_t id) {
    auto node = m_graph.newNode(id);
    m_idToNode[id] = node;
    m_nodes.push_back(GraphNode{.id = id, .x = 0, .y = 0});
}
void LayoutEngine::removeNode(uint32_t id) {
    auto it = m_idToNode.find(id);
    if (it == m_idToNode.end()) {
        return;
    }
    m_graph.delNode(it->second);
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
    auto fromNode = m_idToNode.find(from);
    auto toNode = m_idToNode.find(to);
    if (fromNode == m_idToNode.end() || toNode == m_idToNode.end()) {
        return;
    }
    auto edge = m_graph.newEdge(fromNode->second, toNode->second);
    std::string k = key(from, to);
    m_idToEdge[k] = edge;
    m_edges.push_back(GraphEdge{.key = k, .from = from, .to = to, .bends = {}});
}
void LayoutEngine::removeEdge(const std::string &k) {
    auto it = m_idToEdge.find(k);
    if (it == m_idToEdge.end()) {
        return;
    }
    m_graph.delEdge(it->second);
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
std::string LayoutEngine::key(uint32_t from, uint32_t to) {
    return std::to_string(from) + "->" + std::to_string(to);
}
void LayoutEngine::assignFermatSpiralPositions() {
    const double golden = M_PI * (3 - std::sqrt(5));
    const double scale = 50.0;
    for (int i = 0; i < m_nodes.size(); i++) {
        double r = scale * std::sqrt(i + 1);
        double theta = i * golden;
        m_nodes[i].x = r * std::cos(theta);
        m_nodes[i].y = r * std::sin(theta);
    }
}