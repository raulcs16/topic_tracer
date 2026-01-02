#include "fermatspiral_strategy.hpp"

//TODO: add  coordinate translations
void FermatSpiralStrategy::apply(std::vector<GraphNode> &nodes,
                                 std::vector<GraphEdge> &edges,
                                 BoundingBox &bbox) {

    const double golden = M_PI * (3 - std::sqrt(5));
    for (int i = 0; i < nodes.size(); i++) {
        double r = std::sqrt(i + 1);
        double theta = i * golden;
        double x = r * std::cos(theta);
        double y = r * std::sin(theta);

        // double m = std::max(std::abs(x), std::abs(y));
        // if (m > 0.0) {
        //     x /= m;
        //     y /= m;
        // }
        nodes[i].x = x;
        nodes[i].y = y;
        bbox.max_x = std::max(x, bbox.max_x);
        bbox.min_x = std::min(x, bbox.min_x);
        bbox.max_y = std::max(y, bbox.max_y);
        bbox.min_y = std::min(y, bbox.min_y);
    }
    //for each edge find the nodes with matching from and to's
    //set source x to from.x and source y to from.y similarly for target using to
    //ensure bends has them too
    for (auto &edge : edges) {
        auto fromIt = std::find_if(nodes.begin(), nodes.end(), [&](const GraphNode &n) {
            return n.id == edge.from;
        });
        auto toIt = std::find_if(nodes.begin(), nodes.end(), [&](const GraphNode &n) {
            return n.id == edge.to;
        });

        if (fromIt != nodes.end() && toIt != nodes.end()) {
            // Update edge endpoints
            edge.source_x = fromIt->x;
            edge.source_y = fromIt->y;
            edge.target_x = toIt->x;
            edge.target_y = toIt->y;

            // Optionally set bends (straight line for now)
            edge.bends.clear();
            ogdf::DPoint src(fromIt->x, fromIt->y);
            ogdf::DPoint dst(toIt->x, toIt->y);
            edge.bends.push_back(src);
            edge.bends.push_back(dst);
        }
    }
}