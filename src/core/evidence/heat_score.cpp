#include "heat_score.hpp"

HeatScoreSystem::HeatScoreSystem(EvidenceDB &evidenceDb, Graph &tg)
    : m_evidenceDb(evidenceDb), m_graph(tg) {}


std::unordered_map<uint32_t, float> HeatScoreSystem::computeProgressScores() const {
    std::unordered_map<uint32_t, float> scores;

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
                                  std::unordered_map<uint32_t, float> &scores,
                                  std::set<uint32_t> &visited) const {

    if (!currentNode || amount < 0.01f || visited.count(currentNode->id)) {
        return;
    }
    visited.insert(currentNode->id);
    scores[currentNode->id] += amount;
    for (auto parent : m_graph.parentsOf(currentNode->id)) {
        propagateUp(parent, amount * m_decayFactor, scores, visited);
    }
}
std::unordered_map<uint32_t, float> HeatScoreSystem::computeStressScores() const {
    std::unordered_map<uint32_t, float> scores;
    for (const auto node : m_graph.nodes()) {
        float score = 0;
        auto outEdges = m_graph.getOutEdges(node->id);
        for (const auto edge : outEdges) {
            switch (edge->type) {
            case EdgeType::Null: score += 5.0f; break;
            case EdgeType::Composes: score += 2.0f; break;
            case EdgeType::Associates: score += 1.0f; break;
            case EdgeType::Aggregates: score += 1.5f; break;
            case EdgeType::Injects: score += 0.5f; break;
            case EdgeType::Implements: score += 0.0f; break;
            }
        }
        float normalized = std::clamp(score / 15.0f, 0.0f, 1.0f);
        scores.emplace(node->id, normalized);
    }
    return scores;
}