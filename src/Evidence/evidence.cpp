#include "evidence.hpp"

void EvidenceDB::addProject(const Project &project) {
    if (project.name.length() == 0)
        return;
    if (project.rootPath.length() == 0)
        return;
    m_projects.insert({project.name, std::move(project)});
}
bool EvidenceDB::addEvidence(const std::string &projectName, const EvidenceItem &item) {
    auto ptr = m_projects.find(projectName);
    if (ptr == m_projects.end())
        return false;
    auto &project = ptr->second;
    project.evidence.push_back(item);
    m_topicIndex[item.topic].push_back(&project.evidence.back());
    return true;
}

const std::vector<EvidenceItem *> &EvidenceDB::getEvidenceForTopic(
    const std::string &topic) {
    static const std::vector<EvidenceItem *> empty;

    auto it = m_topicIndex.find(topic);
    if (it == m_topicIndex.end())
        return empty;

    return it->second;
}
