#pragma once

#include "layout_types.hpp"
#include "ui_states.hpp"
#include <QAbstractListModel>
#include <QObject>
#include <QPointF>
#include <QtQml/qqml.h>
#include <vector>


struct RectData {
    uint32_t id;
    float x, y, w, h;
};

class RectListModel : public QAbstractListModel, public ILayoutObserver {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Use Graph.rects instead")
public:
    enum RectRoles {
        RectX = Qt::UserRole + 1,
        RectY,
        RectW,
        RectH,
        RectId
    };
    RectListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //observers
    void onNodeAdded(const GraphNode &node) override {}
    void onNodeUpdated(const GraphNode &node) override {}
    void onNodeRemoved(uint32_t id) override {}
    void onEdgeAdded(const GraphEdge &edge) override {}
    void onEdgeRemoved(const std::string &key) override {}

    void onClusterRectUpdated(uint32_t clusterId,
                              float x,
                              float y,
                              float w,
                              float h) override;
    void onClear() override;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<RectData> m_rects;
};
