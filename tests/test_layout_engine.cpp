#include "fermatspiral_strategy.hpp"
#include "layout_engine.hpp"
#include <catch2/catch_test_macros.hpp>

LayoutEngine engine;


TEST_CASE("Empty Engine, 1 cluster(pool)", "[LayoutEngine]") {
    REQUIRE(engine.clusterCount() == 1);
}
TEST_CASE("Adding Node, returns GraphNode, 1 cluster", "[LayoutEngine]") {
    uint32_t id = 32;
    engine.addNode(id);
    auto clusters = engine.clusterCount();
    REQUIRE(clusters == 1);
}
TEST_CASE("Joining Pool Nodes, create 2 clusters") {
    engine.addNode(2);
    engine.addNode(3);
    engine.addEdge(2, 3);
    auto clusters = engine.clusterCount();
    REQUIRE(clusters == 2);
}
TEST_CASE("Joining 1  from Pool 1 from NonePool, create 0 clusters") {
    engine.addNode(2);
    engine.addNode(3);
    engine.addEdge(2, 3);
    auto before = engine.clusterCount();
    engine.addNode(4); //new pool node
    engine.addEdge(3, 4);
    auto after = engine.clusterCount();
    REQUIRE(before == after);
}

TEST_CASE("Merging 2 cluster,reduces cluster count,by 1") {
    engine.addNode(2);
    engine.addNode(3);
    engine.addEdge(2, 3);
    engine.addNode(4);
    engine.addNode(5);
    engine.addEdge(4, 5);
    auto before = engine.clusterCount();
    engine.addEdge(4, 3);
    auto after = engine.clusterCount();
    REQUIRE(after == before - 1);
}

TEST_CASE("FermatSpiralStrategy, clamped between [-1,1] on x and y") {
    std::vector<GraphNode> nodes;
    std::vector<GraphEdge> edges;
    for (uint32_t i = 0; i < 100; i++) {
        nodes.push_back(GraphNode{.id{i}, .x{0}, .y{0}});
    }
    FermatSpiralStrategy strategy;
    BoundingBox bbox{.max_x{0}, .max_y{0}, .min_x{0}, .min_y{0}};
    strategy.apply(nodes, edges, bbox);
    REQUIRE(bbox.min_x >= -2);
    REQUIRE(bbox.min_y >= -2);
    REQUIRE(bbox.max_x <= 2);
    REQUIRE(bbox.max_y <= 2);
}