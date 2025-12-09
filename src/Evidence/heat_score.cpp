#include "heat_score.hpp"


HeatScoreSystem::HeatScoreSystem(EvidenceDB &evidenceDb, TopicGraph &tg)
    : m_evidenceDb(evidenceDb), m_topicGraph(tg) {}


int HeatScoreSystem::computeHeatScore(
    const Topic *topic,
    std::unordered_map<const Topic *, int> &memo) const {

    if (!topic)
        return 0;

    if (memo.count(topic)) {
        return memo[topic];
    }

    int score = 0;
    if (topic->type == TopicType::Concept) {
        for (auto tc : m_topicGraph.childrenOf(topic->id)) {
            int childScore = computeHeatScore(tc, memo);
            score += childScore > 0 ? 1 : 0;
        }
    } else {
        const auto &evidence = m_evidenceDb.getEvidenceForTopic(topic->name);
        score += static_cast<int>(evidence.size());
    }
    memo[topic] = score;
    return score;
}

std::unordered_map<const Topic *, int> HeatScoreSystem::computeAllHeatScores() const {
    std::unordered_map<const Topic *, int> heatScores;
    std::unordered_map<const Topic *, int> memo;
    for (auto topic : m_topicGraph.topics()) {
        heatScores[topic] = computeHeatScore(topic, memo);
    }

    return heatScores;
}