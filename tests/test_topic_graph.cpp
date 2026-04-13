#include "graph.hpp"
#include "path_analyzer.hpp"
#include <catch2/catch_test_macros.hpp>


TopicGraph graph;

TEST_CASE("Add Topics, all unique") {

    size_t count = 100;

    for (size_t i = 0; i < count; i++) {
        auto topic = graph.addTopic("Topic" + std::to_string(i));
        REQUIRE(topic != nullptr);
    }
    REQUIRE(graph.topicCount() == count);
}
TEST_CASE("Add only unique topics, ingore duplicates") {
    size_t count = 100;

    for (size_t i = 0; i < count; i++) {
        auto topic = graph.addTopic("Topic" + std::to_string(i));
        REQUIRE(topic != nullptr);
    }
    for (size_t i = 0; i < count; i++) {
        auto topic = graph.addTopic("Topic" + std::to_string(i));
        REQUIRE(topic == nullptr);
    }
    REQUIRE(graph.topicCount() == count);
}
TEST_CASE("Dont Add duplicate Edges") {

    auto topicA = graph.addTopic("TopicA");
    auto topicB = graph.addTopic("TopicB");
    graph.addEdge(topicA, topicB, EdgeType::ComposedOf);
    SECTION("Same Edge Type") {
        auto edge = graph.addEdge(topicA, topicB, EdgeType::ComposedOf);
        REQUIRE(graph.edgeCount() == 1);
    }
    SECTION("Different EdgeTypes") {
        auto edge = graph.addEdge(topicA, topicB, EdgeType::AlternativeTo);
        REQUIRE(graph.edgeCount() == 1);
    }
}
TEST_CASE("Return Parents") {
    auto child = graph.addTopic("Child1");
    auto parent1 = graph.addTopic("Parent1");
    auto parent2 = graph.addTopic("Parent2");
    auto parent3 = graph.addTopic("Parent3");
    graph.addEdge(parent1, child, EdgeType::ComposedOf);
    graph.addEdge(parent2, child, EdgeType::ComposedOf);
    graph.addEdge(parent3, child, EdgeType::ComposedOf);
    auto parents = graph.parentsOf(child->id);
    REQUIRE(parents.size() == 3);
}
TEST_CASE("Return Children") {
    auto parent = graph.addTopic("Parent");
    auto child1 = graph.addTopic("child1");
    auto child2 = graph.addTopic("child2");
    auto child3 = graph.addTopic("child3");
    graph.addEdge(parent, child1, EdgeType::ComposedOf);
    graph.addEdge(parent, child2, EdgeType::ComposedOf);
    graph.addEdge(parent, child3, EdgeType::ComposedOf);
    auto children = graph.childrenOf(parent->id);
    REQUIRE(children.size() == 3);
}