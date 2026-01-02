#pragma once


#include "layout_engine.hpp"
#include "ui_states.hpp"
#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqml.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/GraphObserver.h>
#include <vector>


struct NodeItem {
    uint32_t id;
    QString label;
    int heat = 0;
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

    explicit NodeListModel(QObject *parent = nullptr);

    //abstractlistmodel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    //Incremental API
    void addItem(NodeItem item);

    // bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    void resetNodes(const std::vector<NodeItem> &nodes);

    void setFlagsOnId(uint32_t id, StateFlag flags);
    void unSetFlagsOnId(uint32_t id, StateFlag flags);
    void updateHeatScore(uint32_t id, int score);

    void deleteNode(uint32_t id);
    void updateLabel(uint32_t id, const QString &name);

    void onNodeAdded(const GraphNode &node) override;
    void onNodeRemoved(uint32_t id) override;
    void onEdgeAdded(const GraphEdge &edge) override;
    void onEdgeRemoved(const std::string &edge) override;
    void onClear() override;

public slots:
    void onGaphChanged();

private:
    size_t getIndex(uint32_t id);
    void updatePos(int index, double x, double y);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<NodeItem> m_nodes;
    std::unordered_map<uint32_t, ItemState> m_stateFlags;
};