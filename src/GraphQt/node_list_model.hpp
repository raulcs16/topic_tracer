#pragma once


#include "graph_store.hpp"
#include "layout_types.hpp"
#include "ui_states.hpp"
#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqml.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/GraphObserver.h>
#include <vector>


struct NodeItem {
    uint32_t id;
    float heat = 0.0f;
    double x;
    double y;
};

class NodeListModel : public QAbstractListModel, public ILayoutObserver {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Use TGC.nodes instead")

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        LabelRole,
        XRole,
        YRole,
        FlagsRole,
        HeatRole,
    };

    explicit NodeListModel(GraphStore *store, QObject *parent = nullptr);
    //abstractlistmodel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    //Incremental API

    void updateHeatScore(uint32_t id, float score);

    void onNodeAdded(const GraphNode &node) override;
    void onNodeRemoved(uint32_t id) override;
    void onNodeUpdated(const GraphNode &node) override;
    void onClear() override;

    void onEdgeAdded(const GraphEdge &edge) override {}
    void onEdgeRemoved(const std::string &edge) override {}
    void onClusterRectUpdated(uint32_t clusterId,
                              float x,
                              float y,
                              float w,
                              float h) override {}
    void onClusterRectDeleted(uint32_t) override {}


signals:
    void hoverRequested(uint32_t id, bool isHovered);
    void selectRequested(uint32_t id);
    void toggleSelectionRequest(uint32_t id);

public slots:
    void onLabelUpdated(uint32_t id);
    void onFlagsUpdated(uint32_t id);

private:
    int getIndex(uint32_t id);
    void updatePos(int index, double x, double y);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<NodeItem> m_nodes;
    GraphStore *m_store;
};