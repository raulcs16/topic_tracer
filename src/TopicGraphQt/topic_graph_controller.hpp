#pragma once

#include "edge_list_model.hpp"
#include "layout_engine.hpp"
#include "node_list_model.hpp"
#include "topic_graph.hpp"
#include "topic_list_model.hpp"
#include "ui_states.hpp"
#include <QObject>
#include <QtQml/qqml.h>


class TopicGraphController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(TopicListModel *topicListModel READ topicListModel CONSTANT)
    Q_PROPERTY(EdgeListModel *edgeListModel READ edgeListModel CONSTANT)
    Q_PROPERTY(NodeListModel *nodeListModel READ nodeListModel CONSTANT)


public:
    using Topic_Type = ::Topic_Type;
    using Edge_Type = ::Edge_Type;
    Q_ENUM(Topic_Type);
    Q_ENUM(Edge_Type);

    explicit TopicGraphController(QObject *parent = nullptr);
    ~TopicGraphController();
    TopicListModel *topicListModel() const { return m_topicList; }
    NodeListModel *nodeListModel() const { return m_nodeList; }
    EdgeListModel *edgeListModel() const { return m_edgeList; }


    //QML API

    Q_INVOKABLE void createTopic(const QString &name,
                                 Topic_Type type = Topic_Type::Concept);

    Q_INVOKABLE void deleteTopic(const QString &name);
    Q_INVOKABLE void join(const QString &topicA,
                          const QString &topicB,
                          Edge_Type type = Edge_Type::DependsOn);
    Q_INVOKABLE void noJoin(const QString &topicA, const QString &topicB);
    Q_INVOKABLE void rename(const QString &topic, const QString &new_name);
    Q_INVOKABLE void path(const QString &topicA, const QString &topicB);

    Q_INVOKABLE void defaultLayout();
    Q_INVOKABLE void directedLayout();
    Q_INVOKABLE void treeLayout();
    Q_INVOKABLE void circularLayout();
    Q_INVOKABLE void planarLayout();
    Q_INVOKABLE void multiLayout();


public slots:
    void onStateChanged(const std::string &id, const StateFlags &flags);

    void onTopicHovered(uint32_t id);
    void onTopicUnHovered(uint32_t id);
    void onTopicSelected(uint32_t id);
    void onTopicUnSelected(uint32_t id);
    // public slots:
    //     void addTopic(int tempId, const QString &name, Topic_Type type = Topic_Type::Concept);
    //     void renameTopic(uint32_t id, const QString &new_name);
    //     void deleteTopic(uint32_t id);

    // private slots:
    //     void onRequestAddTopic(int tempId, const QString &name);

    // signals:
    //     void topicAdded(int tempId, bool success, uint32_t id);
    //     void topicDelelted(bool success, uint32_t id);
    //     void topicRenamed(bool success, uint32_t id);

    //     void topicListModelChanged();
    //     void graphControllerChanged();
private:
    void synchGraphView();

private:
    TopicGraph m_graph;
    LayoutEngine m_layout;

    TopicListModel *m_topicList;
    NodeListModel *m_nodeList;
    EdgeListModel *m_edgeList;
};