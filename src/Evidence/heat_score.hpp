#pragma once
#include "evidence.hpp"
#include "topic_graph.hpp"
#include <set>

class HeatScoreSystem {

public:
    HeatScoreSystem(EvidenceDB &evidenceDb, TopicGraph &topicGraph);
    std::unordered_map<const Topic *, float> computeAllHeatScores() const;

private:
    void propagateUp(const Topic *currentNode,
                     float amount,
                     std::unordered_map<const Topic *, float> &scores,
                     std::set<uint32_t> &visited) const;

private:
    float m_decayFactor = 0.5f;
    EvidenceDB &m_evidenceDb;
    TopicGraph &m_topicGraph;
};