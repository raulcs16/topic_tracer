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

    graph.addEdge(v2, v1, Edge_Type::ComposedOf);
    graph.addEdge(v1, v3, Edge_Type::Example);
    graph.addEdge(v3, v2, Edge_Type::DependsOn);
    graph.addEdge(v3, v5, Edge_Type::AlternativeTo);
    graph.addEdge(v3, v4, Edge_Type::DependsOn);
    graph.addEdge(v5, v2, Edge_Type::ComposedOf);
    graph.addEdge(v5, v6, Edge_Type::AlternativeTo);
    graph.addEdge(v6, v4, Edge_Type::Example);

    SECTION("TEST SHORTEST PATH") {
        auto path = PathAnalyzer::dijsktras(graph, v1, v2);


        REQUIRE(path.find(v2) != path.end());
        REQUIRE(path[v2] == v5);
        REQUIRE(path[v1] == -1);
    }
}
