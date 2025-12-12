#include "edge_store.hpp"
#include "path_analyzer.hpp"
#include "topic_graph.hpp"
#include "topic_store.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("TopicGraphTest") {

    TopicStore topics;
    EdgeStore edges;
    TopicGraph graph(topics, edges);

    // Build some topics
    auto c1 = graph.addTopic("Concept1", TopicType::Concept);
    auto c2 = graph.addTopic("Concept2", TopicType::Concept);
    auto c3 = graph.addTopic("Concept3", TopicType::Concept);

    auto x1 = graph.addTopic("Concrete1", TopicType::Concrete);
    auto x2 = graph.addTopic("Concrete2", TopicType::Concrete);
    auto x3 = graph.addTopic("Concrete3", TopicType::Concrete);
    SECTION("normalizeJoin ") {
        {
            auto [parent, child] = graph.normalizeJoin(x1, x2, EdgeType::AlternativeTo);
            REQUIRE(parent->id == x1->id);
            REQUIRE(child->id == x2->id);
        }
        {

            auto [parent, child] = graph.normalizeJoin(c1, c2, EdgeType::AlternativeTo);
            REQUIRE(parent->id == c1->id);
            REQUIRE(child->id == c2->id);
        }
        {


            auto [parent, child] = graph.normalizeJoin(c2, x2, EdgeType::DependsOn);

            REQUIRE(parent->id == c2->id);
            REQUIRE(child->id == x2->id);
        }
        {

            auto [parent, child] = graph.normalizeJoin(x2, c2, EdgeType::Example);

            REQUIRE(parent->id == c2->id);
            REQUIRE(child->id == x2->id);
        }
        {

            auto [parent, child] = graph.normalizeJoin(c2, x2, EdgeType::Example);

            REQUIRE(parent->id == c2->id);
            REQUIRE(child->id == x2->id);
        }
    }
    SECTION("Example must be Concrete → Concept") {
        REQUIRE(graph.edgeCount() == 0);
        // Valid cases
        REQUIRE(graph.addEdge(c2, x2, EdgeType::Example) != nullptr);
        REQUIRE(graph.addEdge(x1, c1, EdgeType::Example) != nullptr);
        // Invalid: Concept → Concept
        REQUIRE(graph.addEdge(c1, c2, EdgeType::Example) == nullptr);
        // Invalid: Concrete → Concrete
        REQUIRE(graph.addEdge(x1, x2, EdgeType::Example) == nullptr);
    }

    SECTION("ComposedOf must be Concept → Concept and acyclic") {
        REQUIRE(graph.edgeCount() == 0);
        // Valid
        REQUIRE(graph.addEdge(c1, c2, EdgeType::ComposedOf) != nullptr);
        // Invalid type mismatch
        REQUIRE(graph.addEdge(c1, x1, EdgeType::ComposedOf) == nullptr);
        // Add a chain to test cycles
        REQUIRE(graph.addEdge(c2, c3, EdgeType::ComposedOf) != nullptr);
        // Cycle: c3 → c1 should be rejected
        REQUIRE(graph.addEdge(c3, c1, EdgeType::ComposedOf) == nullptr);
    }

    SECTION("DependsOn requires Concrete → Concrete with same parent") {
        REQUIRE(graph.edgeCount() == 0);
        // First create a concept parent and attach children
        auto parent = graph.addTopic("Parent", TopicType::Concept);
        REQUIRE(graph.addEdge(x1, parent, EdgeType::Example) != nullptr);
        REQUIRE(graph.addEdge(x2, parent, EdgeType::Example) != nullptr);
        REQUIRE(graph.sameParent(x1, x2));
        // Valid
        REQUIRE(graph.addEdge(x1, x2, EdgeType::DependsOn) != nullptr);
        // Invalid, Not same parent
        REQUIRE(graph.addEdge(x1, x3, EdgeType::DependsOn) == nullptr);
    }

    SECTION("AlternativeTo requires Concrete → Concrete same parent") {
        REQUIRE(graph.edgeCount() == 0);
        auto parent = graph.addTopic("Parent2", TopicType::Concept);
        REQUIRE(graph.addEdge(x2, parent, EdgeType::Example) != nullptr);
        REQUIRE(graph.addEdge(x3, parent, EdgeType::Example) != nullptr);
        // Valid
        REQUIRE(graph.addEdge(x2, x3, EdgeType::AlternativeTo) != nullptr);
        // Wrong cluster
        REQUIRE(graph.addEdge(x1, x2, EdgeType::AlternativeTo) == nullptr);
    }

    SECTION("RelatedTo requires matching topic types") {
        REQUIRE(graph.edgeCount() == 0);
        // Concept ↔ Concept valid
        REQUIRE(graph.addEdge(c1, c2, EdgeType::RelatedTo) != nullptr);
        // Concrete ↔ Concrete valid
        REQUIRE(graph.addEdge(x1, x2, EdgeType::RelatedTo) != nullptr);
        // Invalid: Concept ↔ Concrete
        REQUIRE(graph.addEdge(c1, x1, EdgeType::RelatedTo) == nullptr);
    }

    SECTION("No duplicate edges allowed") {
        REQUIRE(graph.edgeCount() == 0);
        REQUIRE(graph.addEdge(c1, c2, EdgeType::ComposedOf) != nullptr);
        REQUIRE(graph.addEdge(c1, c2, EdgeType::ComposedOf) == nullptr);
    }

    SECTION("No self-loops") {
        REQUIRE(graph.edgeCount() == 0);
        REQUIRE(graph.addEdge(c1, c1, EdgeType::ComposedOf) == nullptr);
        REQUIRE(graph.addEdge(x1, x1, EdgeType::DependsOn) == nullptr);
    }
}