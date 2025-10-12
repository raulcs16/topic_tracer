#pragma once


#include "graph_engine.hpp"
#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqml.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/GraphObserver.h>
#include <vector>


class GraphController;


class NodeListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Use Graph.nodes instead")

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        XRole,
        YRole,
    };

    explicit NodeListModel(GraphController *graph);

    //abstractlistmodel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    // bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    void resetNodes(const std::vector<GraphNode> &nodes);
public slots:
    void onGaphChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    GraphController *m_controller;

    std::vector<GraphNode> m_nodes;
};