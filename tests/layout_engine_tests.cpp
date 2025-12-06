#include "layout_engine.hpp"
#include <catch2/catch_test_macros.hpp>


TEST_CASE("LayoutTest") {
    LayoutEngine engine;
    SECTION("addEdge throw invalid arguemt") { REQUIRE_THROWS(engine.addEdge(1, 2)); }
    SECTION("GraphData should return 2 nodes 1 edge") {
        engine.addNode(1);
        engine.addNode(2);
        auto result = engine.addEdge(1, 2);
        auto expectedNodes = 2;
        auto expectedEdges = 1;
        REQUIRE(result.nodes.size() == expectedNodes);
        REQUIRE(result.edges.size() == expectedEdges);
    }
}
