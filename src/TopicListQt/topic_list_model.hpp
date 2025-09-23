#pragma once

#include "topic_graph.hpp"
#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqml.h>


class TopicListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("TopicList should be 'wired' up in .cpp file first")

public:
    explicit TopicListModel(TopicGraph &graph, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

protected:
    QHash<int, QByteArray> roleNames() const override;


private:
    //holds ref to read/write model
    //todo make read only interface?
    TopicGraph &m_graph;
};
