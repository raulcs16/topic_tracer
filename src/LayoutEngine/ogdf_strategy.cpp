#include "ogdf_strategy.hpp"


void OGDFStrat::apply(std::vector<GraphNode> &nodes, std::vector<GraphEdge> &edges) {
    if (!m_layout)
        return;
    try {
        m_layout->call(m_ctx.attributes);
        m_ctx.attributes.addNodeCenter2Bends();
        for (auto &node : nodes) {
            auto it = m_ctx.idToNode.find(node.id);
            if (it == m_ctx.idToNode.end())
                continue;
            node.x = m_ctx.attributes.x(it->second);
            node.y = m_ctx.attributes.y(it->second);
        }
        for (auto &edge : edges) {
            auto src = m_ctx.idToNode.find(edge.from);
            auto target = m_ctx.idToNode.find(edge.to);
            if (src == m_ctx.idToNode.end() || target == m_ctx.idToNode.end())
                continue;

            edge.source_x = m_ctx.attributes.x(src->second);
            edge.source_y = m_ctx.attributes.y(src->second);
            edge.target_x = m_ctx.attributes.x(target->second);
            edge.target_y = m_ctx.attributes.y(target->second);
            auto e = m_ctx.keyToEdge.find(edge.key);
            if (e == m_ctx.keyToEdge.end())
                continue;
            edge.bends.clear();
            for (ogdf::DPoint &p : m_ctx.attributes.bends(e->second)) {
                edge.bends.push_back(p);
            }
        }

    } catch (ogdf::AlgorithmFailureException &e) {
        std::cerr << "Algorithmn Failure: " << (int)(e.exceptionCode()) << "\n";
    } catch (...) {
        std::cerr << "unknown error\n";
    }
}