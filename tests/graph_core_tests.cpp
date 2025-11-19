#include "path_analyzer.hpp"
#include "topic_graph.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Path Test Suite") {
    TopicGraph graph;
    auto v1 = graph.addTopic("v1");
    auto v2 = graph.addTopic("v2");
    auto v3 = graph.addTopic("v3");
    auto v4 = graph.addTopic("v4");
    auto v5 = graph.addTopic("v5");
    auto v6 = graph.addTopic("v1");

    graph.addEdge(v2, v1, EdgeType::ComposedOf);
    graph.addEdge(v1, v3, EdgeType::Example);
    graph.addEdge(v3, v2, EdgeType::DependsOn);
    graph.addEdge(v3, v5, EdgeType::AlternativeTo);
    graph.addEdge(v3, v4, EdgeType::DependsOn);
    graph.addEdge(v5, v2, EdgeType::ComposedOf);
    graph.addEdge(v5, v6, EdgeType::AlternativeTo);
    graph.addEdge(v6, v4, EdgeType::Example);

    SECTION("TEST SHORTEST PATH") {
        auto parents = PathAnalyzer::dijsktras(graph, v1, v2);
        auto path = PathAnalyzer::topicPath(parents, v2);

        REQUIRE(path.size() == 3);
        REQUIRE(path[0] == v1);
        REQUIRE(path[1] == v3);
        REQUIRE(path[2] == v2);
    }
}
