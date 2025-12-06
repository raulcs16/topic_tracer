#include "ogdf_strategy.hpp"


OGDFStrategy::OGDFStrategy() {}
void OGDFStrategy::apply(std::vector<GraphNode> &nodes,
                         std::vector<GraphEdge> &edges,
                         BoundingBox &bbox) {
    if (!m_layout)
        return;
    try {
        if (auto ctx = m_ctx.lock()) {
            m_layout->call(ctx->attributes);
            ctx->attributes.addNodeCenter2Bends();
            for (auto &node : nodes) {
                auto it = ctx->idToNode.find(node.id);
                if (it == ctx->idToNode.end())
                    continue;
                double x = ctx->attributes.x(it->second);
                double y = ctx->attributes.y(it->second);
                node.x = x;
                node.y = y;
                bbox.max_x = x > bbox.max_x ? x : bbox.max_x;
                bbox.min_x = x < bbox.min_x ? x : bbox.min_x;
                bbox.max_y = y > bbox.max_y ? y : bbox.max_y;
                bbox.min_y = y < bbox.min_y ? y : bbox.min_y;
            }
            for (auto &edge : edges) {
                auto src = ctx->idToNode.find(edge.from);
                auto target = ctx->idToNode.find(edge.to);
                if (src == ctx->idToNode.end() || target == ctx->idToNode.end())
                    continue;

                edge.source_x = ctx->attributes.x(src->second);
                edge.source_y = ctx->attributes.y(src->second);
                edge.target_x = ctx->attributes.x(target->second);
                edge.target_y = ctx->attributes.y(target->second);
                auto e = ctx->keyToEdge.find(edge.key);
                if (e == ctx->keyToEdge.end())
                    continue;
                edge.bends.clear();
                for (ogdf::DPoint &p : ctx->attributes.bends(e->second)) {
                    edge.bends.push_back(p);
                }
            }
        }
    } catch (ogdf::AlgorithmFailureException &e) {
        std::cerr << "Algorithmn Failure: " << (int)(e.exceptionCode()) << "\n";
    } catch (...) {
        std::cerr << "unknown error\n";
    }
}
void OGDFStrategy::setContext(std::weak_ptr<OGDFContext> context) { m_ctx = context; }