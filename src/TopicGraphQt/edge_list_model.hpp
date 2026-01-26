#pragma once


#include "layout_engine.hpp"
#include "topic_graph_store.hpp"
#include "ui_states.hpp"
#include <QAbstractListModel>
#include <QObject>
#include <QPointF>
#include <QtQml/qqml.h>
#include <vector>

struct EdgeItem {
    std::string key;
    uint32_t from;
    uint32_t to;
    double source_x;
    double source_y;
    double target_x;
    double target_y;
    std::vector<QPointF> bends;
};

class EdgeListModel : public QAbstractListModel, public ILayoutObserver {
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
        BendsRole,
        FlagsRole,
    };

    explicit EdgeListModel(TGStore *store, QObject *parent = nullptr);

    //abstractlistmodel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    // bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    void addItem(EdgeItem edge);
    void deleteEdge(const std::string &key);

    //layout engine observer
    void onNodeAdded(const GraphNode &node) override;
    void onNodeRemoved(uint32_t id) override;
    void onEdgeAdded(const GraphEdge &edge) override;
    void onEdgeRemoved(const std::string &key) override;
    void onClear() override;

protected:
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void onFlagUpdated(const std::string &key);

private:
    size_t getIndex(const std::string &key);
    void updatePos(int index, const GraphEdge &edge);
    EdgeItem extract(const GraphEdge &edge);

private:
    std::vector<EdgeItem> m_edges;
    TGStore *m_tgstore;
};