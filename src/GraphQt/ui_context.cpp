#include "ui_context.hpp"

UIContext::UIContext(GraphStore *store, QObject *parent)
    : QObject(parent), m_store(store) {
    m_selectionManager = new SelectionManager(store, this);
    m_labels = new LabelListModel(store, m_selectionManager, this);
    m_edges = new EdgeListModel(store, this);
    m_nodes = new NodeListModel(store, this);
    m_rects = new RectListModel(this);
}
void UIContext::setMode(ViewMode mode) {
    if (mode == m_mode)
        return;
    m_mode = mode;
    emit modeChanged();
}
void UIContext::setHoveredNode(uint32_t id, bool isHovered, uint32_t bbId) {
    m_store->setNodeState(id, StateFlag::Hovered, isHovered);
    if (isHovered) {
        m_hoveredId = id;
    } else if (m_hoveredId == id) {
        m_hoveredId = -1;
    } else {
        return;
    }
    m_rects->setSceneBounds(bbId);
}
