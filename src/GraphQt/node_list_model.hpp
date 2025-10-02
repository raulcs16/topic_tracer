#pragma once

//https://github.com/schulzch/qml-ogdf/blob/master/ogdfplugin/nodemodel.h


#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqml.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/GraphObserver.h>

class Graph;


class NodeListModel : public QAbstractListModel, public ogdf::GraphObserver {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Use Graph.nodes instead")
    Q_PROPERTY(int count READ count NOTIFY countChanged);

public:
    enum Roles {
        XRole = Qt::UserRole + 1,
        YRole,
        WidthRole,
        HeightRole
    };

    NodeListModel(Graph *graph);

    //abstractlistmodel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    //graphobserver interface
    void cleared() override;
    void nodeAdded(ogdf::node v) override;
    void nodeDeleted(ogdf::node v) override;
    void edgeAdded(ogdf::edge e) override;
    void edgeDeleted(ogdf::edge e) override;

    //
    void attributesChanged();
    void reInit();

    //qml
    int count() const;
    Q_INVOKABLE QString get(int index) const;
    Q_INVOKABLE void insert(const QString &node);
    Q_INVOKABLE void remove(const QString &node);
signals:
    void countChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    Graph *m_graph;
    QVector<ogdf::node> m_nodes;
};