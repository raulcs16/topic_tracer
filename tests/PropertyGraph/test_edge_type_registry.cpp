#include "edge_type_registry.hpp"
#include "edges.hpp"
#include <catch2/catch_test_macros.hpp>
#include <random>

TEST_CASE("Register Edge Label") {
    EdgeTypeRegistry reg;
    std::vector<AllowedDir> pairs{{1, 2}};
    auto created = reg.create("inherits_from", pairs);
    REQUIRE(created == true);
    REQUIRE(reg.has("inherits_from") == true);
}
TEST_CASE("Don't allow duplicate edge lables") {
    EdgeTypeRegistry reg;
    auto created = reg.create("inherits_from", {});
    auto duplicated = reg.create("inherits_from", {});
    REQUIRE(created == true);
    REQUIRE(duplicated == false);
    REQUIRE(reg.size() == 1);
}
TEST_CASE("Ingore duplicate Allowed Directions per definition") {
    EdgeTypeRegistry reg;
    std::vector<AllowedDir> dir{{1, 2}, {2, 1}, {1, 2}};
    auto created = reg.create("_", dir);
    auto edgeDef = reg.get("_");
    REQUIRE(edgeDef != nullptr);
    REQUIRE(edgeDef->allowed_dir.size() == 2);
}
TEST_CASE("ensure the added directions are on the definition") {
    EdgeTypeRegistry reg;
    reg.create("implements", {{0, 1}});
    auto def = reg.get("implements");
    REQUIRE(def != nullptr);
    REQUIRE(def->allowed_dir.size() == 1);
    REQUIRE(def->allowed_dir.count(AllowedDir{.from = 0, .to = 1}) > 0);
}