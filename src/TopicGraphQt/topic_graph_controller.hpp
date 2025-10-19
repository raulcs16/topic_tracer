#pragma once

#include "graph_controller.hpp"
#include "topic_graph.hpp"
#include "topic_list_model.hpp"
#include <QObject>
#include <QtQml/qqml.h>


class TopicGraphController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(TopicListModel *topicListModel READ topicListModel WRITE setTopicListModel
                   NOTIFY topicListModelChanged)

    Q_PROPERTY(GraphController *graphController READ graphController WRITE
                   setGraphController NOTIFY graphControllerChanged)

public:
    using Topic_Type = ::Topic_Type;
    using Edge_Type = ::Edge_Type;
    Q_ENUM(Topic_Type);
    Q_ENUM(Edge_Type);

    explicit TopicGraphController(QObject *parent = nullptr);
    ~TopicGraphController();
    TopicListModel *topicListModel() const { return m_topicList; }
    GraphController *graphController() const { return m_gr_cntrl; }

    Q_INVOKABLE void createTopic(const QString &name, Topic_Type = Topic_Type::Concept);
    Q_INVOKABLE void join(const QString &topicA, const QString &topicB);


public slots:
    void addTopic(int tempId, const QString &name, Topic_Type type = Topic_Type::Concept);
    void renameTopic(uint32_t id, const QString &new_name);
    void deleteTopic(uint32_t id);

    void setTopicListModel(TopicListModel *list);
    void setGraphController(GraphController *controller);

private slots:
    void onRequestAddTopic(int tempId, const QString &name);

signals:
    void topicAdded(int tempId, bool success, uint32_t id);
    void topicDelelted(bool success, uint32_t id);
    void topicRenamed(bool success, uint32_t id);

    void topicListModelChanged();
    void graphControllerChanged();

private:
    TopicGraph *m_graph;
    GraphController *m_gr_cntrl;
    TopicListModel *m_topicList;
};