#pragma once

#include "topic_graph.hpp"
#include <QObject>
#include <QtQml/qqml.h>


class TopicGraphController : public QObject {
    Q_OBJECT
    QML_ELEMENT

public:
    using Topic_Type = ::Topic_Type;
    using Edge_Type = ::Edge_Type;
    Q_ENUM(Topic_Type);
    Q_ENUM(Edge_Type);

    explicit TopicGraphController(QObject *parent = nullptr);
    ~TopicGraphController();


public slots:
    void addTopic(int tempId, const QString &name, Topic_Type type = Topic_Type::Concept);
    void renameTopic(uint32_t id, const QString &new_name);
    void deleteTopic(uint32_t id);

signals:
    void topicAdded(int tempId, bool success, uint32_t id);
    void topicDelelted(bool success, uint32_t id);
    void topicRenamed(bool success, uint32_t id);

private:
    TopicGraph *m_graph;
};