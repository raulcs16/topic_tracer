#pragma once

#include "edge_list_model.hpp"
#include "graph_layout.hpp"
#include "node_list_model.hpp"

#include <QJSValue>
#include <QObject>
#include <QUrl>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

//https : //github.com/schulzch/qml-ogdf/blob/master/ogdfplugin/graph.h

class Graph : public QObject {
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(GraphLayout *layout READ layout CONSTANT)
    Q_PROPERTY(NodeModel *nodes READ nodes CONSTANT)
    Q_PROPERTY(EdgeModel *edges READ edges CONSTANT)
public:
    Graph(QObject *parent = 0);

    QUrl source() const;
    void setSource(const QUrl &source);

    GraphLayout *layout() const;

    NodeListModel *nodes();
    EdgeListModel *edges();

    // C++ API
    ogdf::Graph &g();

    ogdf::node v(const QString &node) const;
    QString node(ogdf::node v) const;
    void insertNode(ogdf::node v, const QString &node);
    void removeNode(ogdf::node v);
    void clearNodes();

    ogdf::edge e(const QString &edge) const;
    QString edge(ogdf::edge e) const;
    void insertEdge(ogdf::edge e, const QString &edge);
    void removeEdge(ogdf::edge e);
    void clearEdges();

    ogdf::GraphAttributes &attributes();

    // QML API
    Q_INVOKABLE bool save(const QUrl &url);
    Q_INVOKABLE bool reload();
    Q_INVOKABLE void randomGraph(int n, int m);
    Q_INVOKABLE void randomSimpleGraph(int n, int m);
    Q_INVOKABLE void randomBiconnectedGraph(int n, int m);
    Q_INVOKABLE void randomTriconnectedGraph(int n, double p1, double p2);
    Q_INVOKABLE void randomTree(int n);
    Q_INVOKABLE void randomTree(int n, int maxDeg, int maxWidth);
    Q_INVOKABLE void randomHierarchy(int n,
                                     int m,
                                     bool planar,
                                     bool singleSource,
                                     bool longEdges);
    Q_INVOKABLE void randomDiGraph(int n, double p);
    Q_INVOKABLE void clear();

signals:
    void sourceChanged();

private slots:
    void updateModels();

private:
    Q_DISABLE_COPY(Graph)

    QUrl m_source;

    ogdf::Graph m_graph;

    NodeListModel m_nodeModel;
    QHash<ogdf::node, QString> m_nodes;
    QHash<QString, ogdf::node> m_vs;

    EdgeListModel m_edgeModel;
    QHash<ogdf::edge, QString> m_edges;
    QHash<QString, ogdf::edge> m_es;

    ogdf::GraphAttributes m_attributes;
    QScopedPointer<GraphLayout> m_layout;
};
