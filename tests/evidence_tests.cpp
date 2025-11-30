#include "evidence.hpp"
#include "heat_score.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Evidence test SUIT") {
    TopicGraph tg;
    EvidenceDB db;
    HeatScoreSystem heat(db, tg);

    Project A{.evidence{}, .name = "ProjectA", .rootPath = "./root/dir"};
    Project B{.evidence{}, .name = "ProjectB", .rootPath = "./root/dir"};
    Project C{.evidence{}, .name = "ProjectC", .rootPath = "./root/dir"};

    db.addProject(A);
    db.addProject(B);
    db.addProject(C);

    auto ca = tg.addTopic("ConceptA", TopicType::Concept);
    auto cb = tg.addTopic("ConceptB", TopicType::Concept);
    auto cc = tg.addTopic("ConceptC", TopicType::Concept);
    auto cd = tg.addTopic("ConceptD", TopicType::Concept);
    auto ce = tg.addTopic("ConceptE", TopicType::Concept);

    auto a1 = tg.addTopic("ConcreteA1", TopicType::Concrete);
    auto a2 = tg.addTopic("ConcreteA2", TopicType::Concrete);
    auto a3 = tg.addTopic("ConcreteA3", TopicType::Concrete);
    auto b1 = tg.addTopic("ConcreteB1", TopicType::Concrete);
    auto b2 = tg.addTopic("ConcreteB2", TopicType::Concrete);
    auto c1 = tg.addTopic("ConcreteC1", TopicType::Concrete);

    auto d1 = tg.addTopic("ConcreteD1", TopicType::Concrete);
    auto d2 = tg.addTopic("ConcreteD2", TopicType::Concrete);
    auto d3 = tg.addTopic("ConcreteD3", TopicType::Concrete);
    auto e1 = tg.addTopic("ConcreteE1", TopicType::Concrete);
    auto e2 = tg.addTopic("ConcreteE2", TopicType::Concrete);

    SECTION("Calculate HeatScore for Topics without Edges") {
        EvidenceItem item{.filePath = "file/path",
                          .lineStart = 0,
                          .lineEnd = 20,
                          .signature = "abd",
                          .topic = d1->name};
        db.addEvidence(A.name, item);
        for (int i = 0; i < 100; i++) {
            db.addEvidence(A.name, EvidenceItem{.topic = a1->name});
        }
        auto scores = heat.computeAllHeatScores();
        REQUIRE(scores.size() == tg.topicCount());
        REQUIRE(scores[d1.get()] == 1);
        REQUIRE(scores[a1.get()] == 100);
    }
    SECTION("Calculate Concept Score based off children") {
        tg.addEdge(ca.get(), a1.get(), EdgeType::Example);
        tg.addEdge(ca.get(), a2.get(), EdgeType::Example);
        tg.addEdge(ca.get(), a3.get(), EdgeType::Example);

        EvidenceItem ea1_1{.filePath = "//",
                           .lineStart = 0,
                           .lineEnd = 0,
                           .signature = "sig",
                           .topic = a1->name};
        EvidenceItem ea1_2{.filePath = "//",
                           .lineStart = 0,
                           .lineEnd = 0,
                           .signature = "sig",
                           .topic = a1->name};

        EvidenceItem ea2_1 = {.filePath = "//",
                              .lineStart = 0,
                              .lineEnd = 0,
                              .signature = "sig",
                              .topic = a2->name};

        db.addEvidence(A.name, ea1_1);
        db.addEvidence(A.name, ea1_2);
        db.addEvidence(A.name, ea2_1);

        auto scores = heat.computeAllHeatScores();

        REQUIRE(scores.size() == tg.topicCount());
        REQUIRE(scores[ca.get()] == 2);
    }

    SECTION("Calculate Concept Score based off children") {
        tg.addEdge(cb.get(), ca.get(), EdgeType::ComposedOf);
        tg.addEdge(ca.get(), a1.get(), EdgeType::Example);
        tg.addEdge(ca.get(), a2.get(), EdgeType::Example);
        tg.addEdge(ca.get(), a3.get(), EdgeType::Example);

        EvidenceItem ea1_1{.filePath = "//",
                           .lineStart = 0,
                           .lineEnd = 0,
                           .signature = "sig",
                           .topic = a1->name};
        EvidenceItem ea1_2{.filePath = "//",
                           .lineStart = 0,
                           .lineEnd = 0,
                           .signature = "sig",
                           .topic = a1->name};

        EvidenceItem ea2_1{.filePath = "//",
                           .lineStart = 0,
                           .lineEnd = 0,
                           .signature = "sig",
                           .topic = a2->name};

        db.addEvidence(A.name, ea1_1);
        db.addEvidence(A.name, ea1_2);
        db.addEvidence(A.name, ea2_1);

        auto scores = heat.computeAllHeatScores();

        REQUIRE(scores.size() == tg.topicCount());
        REQUIRE(scores[cb.get()] == 1);
    }
}