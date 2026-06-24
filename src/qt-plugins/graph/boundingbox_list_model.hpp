#pragma once

#include "node_type_presentor.hpp"
#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqml.h>


class BoundingBoxListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("cpp Model")

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        LabelRole,
        RectRole,
    };

    explicit BoundingBoxListModel(NodeTypePresentor *presentor);

    //abstractlistmodel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void onBoxAdded(uint32_t id);
    void onBoxRemoved(uint32_t id);
    void onBoxUpdated(uint32_t id);
    void onClear();

private:
    int getIndex(uint32_t);

private:
    QVector<uint32_t> m_ids;
    NodeTypePresentor *m_presentor;
};