#include "pgraph.hpp"
#include <catch2/catch_test_macros.hpp>
#include <random>


auto nreg = std::make_shared<NodeRegistry>();
auto ereg = std::make_shared<EdgeRegistry>();
PGraph graph(nreg, ereg);
TEST_CASE("PGraph::Add Nodes if class type exits") {
    nreg->create("Class");
    REQUIRE(graph.addNode(nreg->get("Class")->typeId, "graph"));
    REQUIRE(graph.addNode(-1, "dne") == false);
}
TEST_CASE("PGraph::Add Edge if allowed dir") {
    nreg->create("Class");
    nreg->create("Interface");
    auto classType = nreg->get("Class");
    auto intType = nreg->get("Interface");

    REQUIRE(classType != nullptr);
    REQUIRE(intType != nullptr);

    ereg->create("implements", {{classType->typeId, intType->typeId}});
    auto implDef = ereg->get("implements");

    REQUIRE(implDef != nullptr);

    REQUIRE(graph.addNode(intType->typeId, "IGraph"));
    REQUIRE(graph.addNode(classType->typeId, "PGraph"));

    auto ig = graph.getNode("IGraph");
    auto pg = graph.getNode("PGraph");

    REQUIRE(ig != nullptr);
    REQUIRE(pg != nullptr);

    REQUIRE(graph.addEdge(implDef->typeId, ig->id, pg->id) == false);
    REQUIRE(graph.addEdge(implDef->typeId, pg->id, ig->id));
}