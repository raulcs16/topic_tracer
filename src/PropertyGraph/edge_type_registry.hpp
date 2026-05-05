#pragma once

#include "edges.hpp"
#include <vector>

class EdgeTypeRegistry {
public:
    bool create(const QString &label, std::vector<AllowedDir> dir = {});
    //IMMUTABLE
    bool has(const QString &label) const;
    bool has(type_id typeId) const;
    std::shared_ptr<const EdgeType> get(const QString &label) const;
    std::shared_ptr<const EdgeType> get(type_id typeId) const;
    inline size_t size() const { return m_labels.size(); }

private:
    std::shared_ptr<EdgeType> find(const QString &label) const;

private:
    type_id m_ref_id = 1;
    std::vector<std::shared_ptr<EdgeType>> m_labels;
};