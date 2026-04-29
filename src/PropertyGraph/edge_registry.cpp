#include "edge_registry.hpp"

bool EdgeRegistry::create(const QString &label, std::vector<AllowedDir> pairs) {
    auto found = find(label);
    if (found != nullptr) {
        return false;
    }
    auto edgl = std::make_shared<EdgeType>();
    edgl->typeId = ++m_ref_id;
    edgl->label = label;
    edgl->allowed_dir = std::set<AllowedDir>(pairs.begin(), pairs.end());
    m_labels.push_back(edgl);
    return true;
}
std::shared_ptr<EdgeType> EdgeRegistry::find(const QString &label) const {
    auto found = std::find_if(m_labels.begin(),
                              m_labels.end(),
                              [&label](const auto &ptr) { return ptr->label == label; });
    if (found == m_labels.end()) {
        return nullptr;
    }
    return (*found);
}
bool EdgeRegistry::has(const QString &label) const { return get(label) != nullptr; }
bool EdgeRegistry::has(type_id typeId) const { return get(typeId) != nullptr; }
std::shared_ptr<const EdgeType> EdgeRegistry::get(const QString &label) const {
    auto found = find(label);
    if (found == nullptr) {
        return nullptr;
    }
    return static_cast<std::shared_ptr<const EdgeType>>(found);
}
std::shared_ptr<const EdgeType> EdgeRegistry::get(type_id typeId) const {
    auto found =
        std::find_if(m_labels.begin(), m_labels.end(), [&typeId](const auto &ptr) {
            return ptr->typeId = typeId;
        });
    if (found == m_labels.end()) {
        return nullptr;
    }
    return static_cast<std::shared_ptr<const EdgeType>>(*found);
}