#pragma once
#include "evidence.hpp"
#include "graph.hpp"
#include <set>

class HeatScoreSystem {

public:
    HeatScoreSystem(EvidenceDB &evidenceDb, Graph &graph);
    std::unordered_map<uint32_t, float> computeProgressScores() const;
    std::unordered_map<uint32_t, float> computeStressScores() const;

private:
    void propagateUp(const Node *currentNode,
                     float amount,
                     std::unordered_map<uint32_t, float> &scores,
                     std::set<uint32_t> &visited) const;

private:
    float m_decayFactor = 0.5f;
    EvidenceDB &m_evidenceDb;
    Graph &m_graph;
};