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