#include "selection_manager.hpp"
#include <QClipboard>
#include <QtGui/qguiapplication.h>

SelectionManager::SelectionManager(GraphStore *store, QObject *parent)
    : QObject(parent), m_store(store) {}

void SelectionManager::clear() {
    for (auto i : m_selectedIds) {
        m_store->setNodeState(i, StateFlag::Selected, false);
    }
    m_selectedIds.clear();
    m_lastSelectedId = -1;
}

void SelectionManager::select(uint32_t id) {
    bool isTogglingOff = m_lastSelectedId == id;
    clear();
    if (!isTogglingOff) {
        m_store->setNodeState(id, StateFlag::Selected, true);
        m_selectedIds.push_back(id);
        m_lastSelectedId = id;
    }
}
void SelectionManager::toggle(uint32_t id) {
    auto it = std::find(m_selectedIds.begin(), m_selectedIds.end(), id);
    if (it == m_selectedIds.end()) {
        m_store->setNodeState(id, StateFlag::Selected, true);
        m_selectedIds.push_back(id);
        m_lastSelectedId = id;
    } else {
        m_store->setNodeState(id, StateFlag::Selected, false);
        m_selectedIds.erase(it);
    }
}
void SelectionManager::selectRange(const std::vector<uint32_t> ids) {
    clear();
    for (const auto id : ids) {
        m_store->setNodeState(id, StateFlag::Selected, true);
        m_selectedIds.push_back(id);
    }
    m_lastSelectedId = *ids.rbegin();
}
void SelectionManager::selectAll() {
    clear();
    m_selectedIds = m_store->setAllNodes(StateFlag::Selected, true);
}
void SelectionManager::copySelection() {
    QClipboard *clipboard = QGuiApplication::clipboard();
    QStringList list;
    for (const auto id : m_selectedIds) {
        auto label = m_store->label(id);
        if (!label.isEmpty()) {
            list.push_back(label);
        }
    }
    QString text = list.join("\n");
    clipboard->setText(text);
}
