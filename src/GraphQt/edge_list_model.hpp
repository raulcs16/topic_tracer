#pragma once

#include "graph_engine.hpp"
#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqml.h>
#include <vector>

class GraphController;

class EdgeListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Use Graph.edges instead")

public:
    enum Roles {
        SourceRole = Qt::UserRole + 1,
        SourceXRole,
        SourceYRole,
        TargetRole,
        TargetXRole,
        TargetYRole,
        BendsRole
    };

    EdgeListModel(GraphController *controller);

    //abstractlistmodel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    // bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    void resetEdges(const std::vector<GraphEdge> &edges);
    // public slots:
    //     void onGraphChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    GraphController *m_controller;
    std::vector<GraphEdge> m_edges;
};