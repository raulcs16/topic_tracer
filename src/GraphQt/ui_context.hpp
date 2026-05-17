#pragma once
#include "boundingbox_list_model.hpp"
#include "edge_list_model.hpp"
#include "graph_store.hpp"
#include "label_list_model.hpp"
#include "node_list_model.hpp"
#include "node_type_presentor.hpp"
#include "selection_manager.hpp"
#include "terminal_list_model.hpp"
#include <QObject>
#include <QtQml/qqml.h>

class UIContext : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Manages UI View Models")
    // Q_PROPERTY(GraphStore *store READ store CONSTANT)
    // Q_PROPERTY(LabelListModel *labelListModel READ labelListModel CONSTANT)
    // Q_PROPERTY(EdgeListModel *edgeListModel READ edgeListModel CONSTANT)
    // Q_PROPERTY(NodeListModel *nodeListModel READ nodeListModel CONSTANT)
    Q_PROPERTY(TerminalListModel *terminalListModel READ terminalListModel CONSTANT)
    Q_PROPERTY(BoundingBoxListModel *bboxListModel READ bboxListModel CONSTANT)
    // Q_PROPERTY(SelectionManager *selectionManager READ selectionManager CONSTANT)
    Q_PROPERTY(ViewMode mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(QString modeName READ modeName NOTIFY modeNameChanged)
public:
    enum class ViewMode {
        Progress,
        Stress
    };
    Q_ENUM(ViewMode)
    explicit UIContext(QObject *parent = nullptr);
    // LabelListModel *labelListModel() const { return m_labels; }
    // EdgeListModel *edgeListModel() const { return m_edges; }
    // NodeListModel *nodeListModel() const { return m_nodes; }
    TerminalListModel *terminalListModel() const { return m_terminal; }
    BoundingBoxListModel *bboxListModel() const { return m_bboxes; }
    // SelectionManager *selectionManager() const { return m_selectionManager; }
    // GraphStore *store() const { return m_store; }

    ViewMode mode() const { return m_mode; }
    void setMode(ViewMode mode);

    QString modeName() const;


signals:
    void modeChanged();
    void modeNameChanged();

private:
    // GraphStore *m_store;
    // SelectionManager *m_selectionManager;
    // LabelListModel *m_labels;
    // EdgeListModel *m_edges;
    // NodeListModel *m_nodes;
    TerminalListModel *m_terminal;
    NodeTypePresentor *m_presentor;
    BoundingBoxListModel *m_bboxes;
    ViewMode m_mode = ViewMode::Progress;
};