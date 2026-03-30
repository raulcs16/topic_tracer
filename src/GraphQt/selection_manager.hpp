#pragma once


#include "graph_store.hpp"
#include <QObject>
#include <QtQml/qqml.h>
#include <vector>

class SelectionManager : public QObject {
    Q_OBJECT

public:
    explicit SelectionManager(GraphStore *store, QObject *parent = nullptr);

    void select(uint32_t id);
    void selectAll();
    void toggle(uint32_t id);
    void selectRange(const std::vector<uint32_t> ids);
    void clear();

    const std::vector<uint32_t> &selectedIds() const { return m_selectedIds; }
    int32_t lastSelectedId() const { return m_lastSelectedId; }

private:
    GraphStore *m_store;
    std::vector<uint32_t> m_selectedIds;
    int32_t m_lastSelectedId = -1;
};
