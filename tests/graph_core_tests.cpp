#include "graph_engine.hpp"
#include <catch2/catch_test_macros.hpp>

bool samePos(const GraphNode &a, const GraphNode &b) { return a.x == b.x && a.y == b.y; }
TEST_CASE("GraphCore Test SUITE") {
    GraphEngine engine;
    GraphState state;
    SECTION("Should have a single node") {
        double initPos = -100;
        state.nodes.push_back(GraphNode{.id = 1, .x = initPos, .y = initPos});
        engine.setState(state);
        engine.calculateLayout();
        auto updatedNodes = engine.nodes();

        REQUIRE(updatedNodes.size() == 1);
        REQUIRE(updatedNodes[0].x != initPos);
        REQUIRE(updatedNodes[0].y != initPos);
    }
    SECTION("Should not place all nodes on same spot") {
        state.nodes.clear();
        for (uint32_t i = 0; i < 10; i++) {
            double initPos = -100;
            state.nodes.push_back(GraphNode{.id = i, .x = initPos, .y = initPos});
        }
        state.edges.push_back(GraphEdge{.from = 1, .to = 3});
        state.edges.push_back(GraphEdge{.from = 1, .to = 4});
        engine.setState(state);
        engine.calculateLayout();
        auto updatedNodes = engine.nodes();
        REQUIRE(!samePos(state.nodes[1], state.nodes[3]));
        REQUIRE(!samePos(state.nodes[1], state.nodes[4]));
        REQUIRE(!samePos(state.nodes[3], state.nodes[4]));
    }
}