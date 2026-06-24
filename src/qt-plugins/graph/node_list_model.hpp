#pragma once


#include "graph_store.hpp"
#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqml.h>

class NodeListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Use TGC.nodes instead")

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        LabelRole,
        PosRole,
        FlagsRole,
        HeatRole,
    };

    explicit NodeListModel(GraphStore *store, QObject *parent = nullptr);
    //abstractlistmodel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

signals:
    void hoverRequested(uint32_t id, bool isHovered);
    void selectRequested(uint32_t id);
    void toggleSelectionRequest(uint32_t id);

public slots:
    void onNodeAdded(uint32_t id);
    void onLabelUpdated(uint32_t id);
    void onFlagsUpdated(uint32_t id);
    void onPosUpdated(uint32_t id);
    void onHeatUpdated(uint32_t id);
    void onNodeDeleted(uint32_t id);
    void onClear();

private:
    int getIndex(uint32_t id);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<uint32_t> m_ids;
    GraphStore *m_store;
};