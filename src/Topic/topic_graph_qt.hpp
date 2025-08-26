#pragma once

#include "topic_graph.hpp"
#include "topic_qt.hpp"
#include <QAbstractListModel>
#include <QQmlEngine>
#include <QSharedPointer>
#include <QVariant>
#include <QVector>

class TopicGraphQt : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("TopicGraphQt model shouldn't be created")

public:
    explicit TopicGraphQt(const std::shared_ptr<TopicGraph> &graph, QObject *parent = nullptr)
        : QAbstractListModel(parent), m_graph(graph) {}

    // QAbstractListModel overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Qt-specific methods to expose your core TopicGraph functionality
    Q_INVOKABLE void addTopic(const QString &name, Topic_Type type);
    Q_INVOKABLE void removeTopic(uint32_t id);
    Q_INVOKABLE void addEdge(uint32_t from, uint32_t to, Edge_Type type);

signals:
    // Any signals you need to emit from the graph model
    void topicAdded(uint32_t id);
    void topicRemoved(uint32_t id);
    void edgeAdded(uint32_t from, uint32_t to);

private:
    std::shared_ptr<TopicGraph> m_graph;
    // A list of TopicQt objects to manage the wrapped data
    QVector<QSharedPointer<TopicQt>> m_topics;
};