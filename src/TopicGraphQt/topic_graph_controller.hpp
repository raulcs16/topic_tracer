#pragma once

#include "topic_graph.hpp"
#include <QObject>
#include <QtQml/qqml.h>


//basically acts as the "server" forwarding request to the core graph model, and then signaling to indicate any changes / failed request
class TopicGraphController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("TopicGraph QOjbect wrapper for graph")

public:
    using Topic_Type = ::Topic_Type;
    using Edge_Type = ::Edge_Type;
    Q_ENUM(Topic_Type);
    Q_ENUM(Edge_Type);

    explicit TopicGraphController(TopicGraph *graph, QObject *parent = nullptr);


public slots:
    void addTopic(const QString &name, Topic_Type type = Topic_Type::Concept);

signals:
    void topicAdded(bool success, uint32_t newId);

private:
    TopicGraph *m_graph;
};