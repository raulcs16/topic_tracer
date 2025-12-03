#include "evidence.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

//TODO:refactor out later
EvidenceDB::EvidenceDB() {
    std::ifstream f("data/evidence.json");
    if (!f)
        return;

    nlohmann::json jsonData;
    f >> jsonData;

    for (auto &proj : jsonData) {
        Project project;
        project.name = proj["name"];
        project.rootPath = proj["rootPath"];

        for (auto &e : proj["evidence"]) {
            EvidenceItem item;
            item.topic = e["topic"];
            item.filePath = e["filePath"];
            item.lineStart = e["lineStart"];
            item.lineEnd = e["lineEnd"];
            item.signature = e["signature"];

            project.evidence.push_back(item);
            m_topicIndex[item.topic].push_back(&project.evidence.back());
        }
        addProject(project);
    }
}
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