#include "heat_score.hpp"

HeatScoreSystem::HeatScoreSystem(EvidenceDB &evidenceDb, TopicGraph &tg)
    : m_evidenceDb(evidenceDb), m_topicGraph(tg) {}


std::unordered_map<const Topic *, float> HeatScoreSystem::computeAllHeatScores() const {
    std::unordered_map<const Topic *, float> scores;

    for (auto topic : m_topicGraph.topics()) {
        const auto &evidence = m_evidenceDb.getEvidenceForTopic(topic->name);

        if (!evidence.empty()) {
            float directHeat = static_cast<float>(evidence.size());
            std::set<uint32_t> visited;
            propagateUp(topic, directHeat, scores, visited);
        }
    }

    return scores;
}
void HeatScoreSystem::propagateUp(const Topic *currentNode,
                                  float amount,
                                  std::unordered_map<const Topic *, float> &scores,
                                  std::set<uint32_t> &visited) const {

    if (!currentNode || amount < 0.01f || visited.count(currentNode->id)) {
        return;
    }
    visited.insert(currentNode->id);
    scores[currentNode] += amount;
    for (auto parent : m_topicGraph.parentsOf(currentNode->id)) {
        propagateUp(parent, amount * m_decayFactor, scores, visited);
    }
}