#include "edge_list_model.hpp"
#include "graph_store.hpp"
#include "label_list_model.hpp"
#include "node_list_model.hpp"
#include "rect_list_model.hpp"
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
public:
    explicit UIContext(GraphStore *store, QObject *parent = nullptr);
    LabelListModel *labelListModel() const { return m_labels; }
    EdgeListModel *edgeListModel() const { return m_edges; }
    NodeListModel *nodeListModel() const { return m_nodes; }
    RectListModel *rectListModel() const { return m_rects; }

private:
    GraphStore *m_store;
    LabelListModel *m_labels;
    EdgeListModel *m_edges;
    NodeListModel *m_nodes;
    RectListModel *m_rects;
};