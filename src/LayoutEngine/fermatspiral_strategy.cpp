#include "fermatspiral_strategy.hpp"

void FermatSpiralStrategy::apply(std::vector<GraphNode> &nodes,
                                 std::vector<GraphEdge> &edges) {

    const double golden = M_PI * (3 - std::sqrt(5));
    const double scale = 50.0;
    for (int i = 0; i < nodes.size(); i++) {
        double r = scale * std::sqrt(i + 1);
        double theta = i * golden;
        nodes[i].x = r * std::cos(theta);
        nodes[i].y = r * std::sin(theta);
    }
}