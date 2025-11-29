#pragma once
#include "evidence.hpp"
#include "topic_graph.hpp"

class HeatScoreSystem {

public:
    HeatScoreSystem(EvidenceDB &evidenceDb, TopicGraph &topicGraph);
    std::unordered_map<const Topic *, int> computeAllHeatScores() const;

private:
    int computeHeatScore(const Topic *,
                         std::unordered_map<const Topic *, int> &memo) const;

private:
    EvidenceDB &m_evidenceDb;
    TopicGraph &m_topicGraph;
};