#include "node_registry.hpp"
#include <catch2/catch_test_macros.hpp>
#include <random>


TEST_CASE("Register Node Type") {
    NodeRegistry reg;
    reg.create("Class");
    REQUIRE(reg.has("Class") == true);
}
TEST_CASE("Register size matches added types", "Core,NodeRegistry") {
    NodeRegistry reg;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 255);
    size_t added = 0;
    size_t missed = 0;
    size_t total = distr(gen);
    for (size_t i = 0; i < total; i++) {
        auto ch = QString::fromStdString(std::to_string(distr(gen)));

        if (reg.create(ch)) {
            added++;
        } else {
            missed++;
        }
    }
    REQUIRE(total == added + missed);
    REQUIRE(added == reg.size());
}