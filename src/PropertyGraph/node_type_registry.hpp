#pragma once

#include "nodes.hpp"

class NodeTypeRegistry {

public:
    std::shared_ptr<const NodeType> create(const QString &label);
    //IMMUTABLE
    std::shared_ptr<const NodeType> get(const QString &label) const;
    std::shared_ptr<const NodeType> get(type_id typeId) const;
    bool has(const QString &label) const;
    inline size_t size() const { return m_types.size(); }

private:
    type_id m_ref_id = 1;
    std::vector<std::shared_ptr<NodeType>> m_types;
};