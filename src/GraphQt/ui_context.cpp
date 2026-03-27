#include "ui_context.hpp"

UIContext::UIContext(GraphStore *store, QObject *parent) {
    m_labels = new LabelListModel(store, this);
    m_edges = new EdgeListModel(store, this);
    m_nodes = new NodeListModel(store, this);
    m_rects = new RectListModel(this);
}