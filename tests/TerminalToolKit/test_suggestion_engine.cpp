#include "suggestion_engine.hpp"
#include <catch2/catch_test_macros.hpp>


class StaticList : public IProvider {
public:
    explicit StaticList(std::vector<std::string> list) : m_list{list} {}

    std::vector<std::string> matches(const std::string &pattern) override {
        std::vector<std::string> result;

        for (const auto &item : m_list) {
            if (item.size() >= pattern.size() &&
                item.compare(0, pattern.size(), pattern) == 0) {
                result.push_back(item);
            }
        }

        return result;
    }

private:
    std::vector<std::string> m_list;
};

TEST_CASE("SuggestionEngineUT") {
    SuggestionEngine se;
    auto fruits = std::make_unique<StaticList>(std::vector<std::string>{"apple",
                                                                        "carrot",
                                                                        "banana",
                                                                        "aprocot",
                                                                        "blueberry",
                                                                        "cramberry"});
    se.registerProvider("fruits", std::move(fruits));
    SECTION("Validate Registration") {
        REQUIRE(se.providerSize() == 1);
        CHECK(se.hasProvider("fruits") == true);
    }
    SECTION("No Provider, no Matches") {
        auto matches = se.matches("cars", "bmw");
        REQUIRE(matches.size() == 0);
    }
    SECTION("Provider, no Matches") {
        auto matches = se.matches("fruits", "peel");
        REQUIRE(matches.size() == 0);
    }
    SECTION("Provider, Match all begining with ap") {
        auto matches = se.matches("fruits", "ap");
        REQUIRE(matches.size() == 2);
    }
}