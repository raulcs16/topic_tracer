#pragma once


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
        BendsRole,
        FlagsRole,
    };

    explicit EdgeListModel(QObject *parent = nullptr);

    //abstractlistmodel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    // bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    void resetEdges(const std::vector<EdgeItem> &edges);
    void setFlagsOnId(const std::string &key, StateFlag flags);
    void unSetFlagsOnId(const std::string &key, StateFlag flags);


protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<EdgeItem> m_edges;
    std::unordered_map<std::string, ItemState> m_stateFlags;
};