#include "heat_score.hpp"

HeatScoreSystem::HeatScoreSystem(EvidenceDB &evidenceDb, Graph &tg)
    : m_evidenceDb(evidenceDb), m_graph(tg) {}


std::unordered_map<const Node *, float> HeatScoreSystem::computeAllHeatScores() const {
    std::unordered_map<const Node *, float> scores;

    for (auto node : m_graph.nodes()) {
        const auto &evidence = m_evidenceDb.getEvidenceForTopic(node->label);

        if (!evidence.empty()) {
            float directHeat = static_cast<float>(evidence.size());
            std::set<uint32_t> visited;
            propagateUp(node, directHeat, scores, visited);
        }
    }

    return scores;
}
void HeatScoreSystem::propagateUp(const Node *currentNode,
                                  float amount,
                                  std::unordered_map<const Node *, float> &scores,
                                  std::set<uint32_t> &visited) const {

    if (!currentNode || amount < 0.01f || visited.count(currentNode->id)) {
        return;
    }
    visited.insert(currentNode->id);
    scores[currentNode] += amount;
    for (auto parent : m_graph.parentsOf(currentNode->id)) {
        propagateUp(parent, amount * m_decayFactor, scores, visited);
    }
}