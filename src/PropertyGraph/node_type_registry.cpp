#include "node_type_registry.hpp"


bool NodeTypeRegistry::create(const QString &label) {
    if (has(label)) {
        return false;
    }
    auto t = std::make_shared<NodeType>();
    t->typeId = ++m_ref_id;
    t->label = label;
    m_types.push_back(t);
    return true;
}
bool NodeTypeRegistry::has(const QString &label) const {
    auto found = get(label);
    return found != nullptr;
}
std::shared_ptr<const NodeType> NodeTypeRegistry::get(const QString &label) const {
    auto found = std::find_if(m_types.begin(), m_types.end(), [&label](const auto &ptr) {
        return ptr->label == label;
    });
    if (found == m_types.end()) {
        return nullptr;
    }
    return static_cast<std::shared_ptr<const NodeType>>(*found);
}
std::shared_ptr<const NodeType> NodeTypeRegistry::get(type_id typeId) const {
    auto found = std::find_if(m_types.begin(), m_types.end(), [&typeId](const auto &ptr) {
        return ptr->typeId == typeId;
    });
    if (found == m_types.end()) {
        return nullptr;
    }
    return static_cast<std::shared_ptr<const NodeType>>(*found);
}