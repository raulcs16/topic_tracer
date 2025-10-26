#pragma once

#include "edge_list_model.hpp"
#include "graph_engine.hpp"
#include "node_list_model.hpp"
#include "topic_graph.hpp"
#include <QObject>
#include <QUrl>
#include <QtQml/qqml.h>


class GraphController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(NodeListModel *nodes READ nodes CONSTANT)
    Q_PROPERTY(EdgeListModel *edges READ edges CONSTANT)

public:
    explicit GraphController(QObject *parent = nullptr);

    NodeListModel *nodes() { return &m_nodeModel; }
    EdgeListModel *edges() { return &m_edgeModel; }

    void synchFromTopicGraph(TopicGraph &graph);
signals:
    void graphChanged();

private:
    GraphEngine m_engine;
    NodeListModel m_nodeModel;
    EdgeListModel m_edgeModel;
};