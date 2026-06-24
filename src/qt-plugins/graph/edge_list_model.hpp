#pragma once
#include "graph_store.hpp"
#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqml.h>


class EdgeListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Use Graph.edges instead")

public:
    enum Roles {
        EdgePosRole = Qt::UserRole + 1,
        FlagsRole,
        EdgeTypeRole,
    };

    explicit EdgeListModel(GraphStore *store, QObject *parent = nullptr);

    //abstractlistmodel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void onEdgeAdded(const std::string &key);
    void onEdgeRemoved(const std::string &key);
    void onFlagUpdated(const std::string &key);
    void onEdgeTypeUpdated(const std::string &key);
    void onPosUpdated(const std::string &key);
    void onClear();

private:
    int getIndex(const std::string &key);

private:
    QVector<std::string> m_ids;
    GraphStore *m_store;
};