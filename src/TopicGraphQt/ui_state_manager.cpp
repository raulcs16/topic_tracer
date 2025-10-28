#include "ui_state_manager.hpp"


UIStateManager::UIStateManager(QObject *parent) : QObject{parent} {}

std::shared_ptr<const UIState> UIStateManager::state(uint32_t id) {
    auto it = m_stateMap.find(id);
    if (it == m_stateMap.end()) {
        return nullptr;
    }
    return it->second;
}
void UIStateManager::setState(uint32_t id, UIState state) {
    m_stateMap[id] = std::make_shared<UIState>(std::move(state));
}
void UIStateManager::setSelectedId(uint32_t id) {
    auto it = m_stateMap.find(id);
    if (it == m_stateMap.end())
        return;
    unsetSelected();
    it->second.get()->selected = true;
    m_selectedId = id;
}
void UIStateManager::unsetSelected() {
    if (m_selectedId == -1)
        return;
    auto it = m_stateMap.find(m_selectedId);
    if (it == m_stateMap.end())
        return;
    it->second.get()->selected = false;
    m_selectedId = -1;
}
void UIStateManager::setHoveredId(int id) {
    if (m_hoveredId == id) {
        return;
    }
    //TODO: loop through all, ensure toggles?
    //reset previously hovered
    auto it = m_stateMap.find(m_hoveredId);
    if (it != m_stateMap.end()) {
        it->second.get()->highlighted = false;
        emit stateChanged(m_hoveredId);
    }
    if (id == -1) {
        m_hoveredId = -1;
        return;
    }
    //set current
    it = m_stateMap.find(id);
    if (it == m_stateMap.end()) {
        return;
    }
    it->second.get()->highlighted = true;
    m_hoveredId = id;
    emit stateChanged(id);
}
