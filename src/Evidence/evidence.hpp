#pragma once

#include <string>
#include <unordered_map>
#include <vector>


struct EvidenceItem {
    std::string topic;
    std::string filePath;
    size_t lineStart;
    size_t lineEnd;
    std::string signature;
};

struct Project {
    std::string name;
    std::string rootPath;
    std::vector<EvidenceItem> evidence;
};

class EvidenceDB {
public:
    void addProject(const Project &project);
    bool addEvidence(const std::string &projectName, const EvidenceItem &item);
    const std::vector<EvidenceItem *> &getEvidenceForTopic(const std::string &topic);
    const std::unordered_map<std::string, std::vector<EvidenceItem *>> topics() {
        return m_topicIndex;
    }

private:
    std::unordered_map<std::string, std::vector<EvidenceItem *>> m_topicIndex;

    std::unordered_map<std::string, Project> m_projects;
};
