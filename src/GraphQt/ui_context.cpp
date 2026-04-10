#include "ui_context.hpp"

UIContext::UIContext(GraphStore *store, QObject *parent)
    : QObject(parent), m_store(store) {
    m_selectionManager = new SelectionManager(store, this);
    m_labels = new LabelListModel(store, m_selectionManager, this);
    m_edges = new EdgeListModel(store, this);
    m_nodes = new NodeListModel(store, this);
    m_bboxes = new BoundingBoxListModel(store, this);
    m_terminal = new TerminalListModel(this);
}
void UIContext::setMode(ViewMode mode) {
    if (mode == m_mode)
        return;
    m_mode = mode;
    emit modeChanged();
    emit modeNameChanged();
}
QString UIContext::modeName() const {
    return m_mode == ViewMode::Progress ? "Progress" : "Stress";
}