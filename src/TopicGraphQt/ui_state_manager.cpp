#include "ui_state_manager.hpp"


UIStateManager::UIStateManager(QObject *parent) : QObject{parent} {}

std::shared_ptr<const UIState> UIStateManager::state(const std::string &id) const {
    auto it = m_stateMap.find(id);
    if (it == m_stateMap.end()) {
        return std::make_shared<UIState>();
    }
    return it->second;
}
void UIStateManager::setState(const std::string &id, StateFlags flags) {
    auto &ptr = m_stateMap[id];
    if (!ptr)
        ptr = std::make_shared<UIState>();
    ptr->flags = flags;
    emit stateChanged(id);
}
void UIStateManager::addState(const std::string &id, StateFlags flags) {
    auto &ptr = m_stateMap[id];
    if (!ptr)
        std::make_shared<UIState>();
    ptr->flags |= flags;
    emit stateChanged(id);
}
void UIStateManager::removeState(const std::string &id, StateFlags flags) {
    auto &ptr = m_stateMap[id];
    if (!ptr)
        return;
    ptr->flags &= ~flags;
    emit stateChanged(id);
}
void UIStateManager::setSelectedId(const std::string &id) {
    auto it = m_stateMap.find(id);
    if (it == m_stateMap.end())
        return;
    removeState(m_selectedId, StateFlag::Selected);
    m_selectedId = id;
    setState(id, StateFlag::Selected);
}

void UIStateManager::setHoveredId(const std::string &id) {
    auto it = m_stateMap.find(id);
    if (it == m_stateMap.end()) {
        return;
    }
    removeState(m_hoveredId, StateFlag::Highlighted);
    m_hoveredId = id;
    setState(id, StateFlag::Highlighted);
}
