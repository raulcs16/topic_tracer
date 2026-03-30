#pragma once
#include "edge_list_model.hpp"
#include "graph_store.hpp"
#include "label_list_model.hpp"
#include "node_list_model.hpp"
#include "rect_list_model.hpp"
#include "selection_manager.hpp"
#include <QObject>
#include <QtQml/qqml.h>

class UIContext : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Manages UI View Models")
    Q_PROPERTY(LabelListModel *labelListModel READ labelListModel CONSTANT)
    Q_PROPERTY(EdgeListModel *edgeListModel READ edgeListModel CONSTANT)
    Q_PROPERTY(NodeListModel *nodeListModel READ nodeListModel CONSTANT)
    Q_PROPERTY(RectListModel *rectListModel READ rectListModel CONSTANT)
    Q_PROPERTY(SelectionManager *selectionManager READ selectionManager CONSTANT)
    Q_PROPERTY(ViewMode mode READ mode WRITE setMode NOTIFY modeChanged)
public:
    enum class ViewMode {
        Progress,
        Stress
    };
    Q_ENUM(ViewMode)
    explicit UIContext(GraphStore *store, QObject *parent = nullptr);
    LabelListModel *labelListModel() const { return m_labels; }
    EdgeListModel *edgeListModel() const { return m_edges; }
    NodeListModel *nodeListModel() const { return m_nodes; }
    RectListModel *rectListModel() const { return m_rects; }
    SelectionManager *selectionManager() const { return m_selectionManager; }
    ViewMode mode() const { return m_mode; }
    void setMode(ViewMode mode);

signals:
    void modeChanged();

private:
    GraphStore *m_store;
    SelectionManager *m_selectionManager;
    LabelListModel *m_labels;
    EdgeListModel *m_edges;
    NodeListModel *m_nodes;
    RectListModel *m_rects;
    ViewMode m_mode = ViewMode::Progress;
};