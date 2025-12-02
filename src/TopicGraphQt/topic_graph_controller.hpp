#pragma once

#include "edge_list_model.hpp"
#include "evidence.hpp"
#include "heat_score.hpp"
#include "layout_engine.hpp"
#include "node_list_model.hpp"
#include "topic_graph.hpp"
#include "topic_graph_repo.hpp"
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
    explicit TopicGraphController(QObject *parent = nullptr);
    ~TopicGraphController();
    TopicListModel *topicListModel() const { return m_topicList; }
    NodeListModel *nodeListModel() const { return m_nodeList; }
    EdgeListModel *edgeListModel() const { return m_edgeList; }

    void calculateHeatScores();
    //QML API

    Q_INVOKABLE void createTopic(const QString &name,
                                 TopicType type = TopicType::Concept);

    Q_INVOKABLE void deleteTopic(const QString &name);
    Q_INVOKABLE void join(const QString &topicA,
                          const QString &topicB,
                          EdgeType type = EdgeType::ComposedOf);
    Q_INVOKABLE void noJoin(const QString &topicA, const QString &topicB);
    Q_INVOKABLE void rename(const QString &topic, const QString &new_name);
    Q_INVOKABLE void path(const QString &topicA, const QString &topicB);

    Q_INVOKABLE void defaultLayout();
    Q_INVOKABLE void directedLayout();
    Q_INVOKABLE void treeLayout();
    Q_INVOKABLE void circularLayout();
    Q_INVOKABLE void planarLayout();
    Q_INVOKABLE void multiLayout();

    Q_INVOKABLE void save(QString fileName);
    Q_INVOKABLE void load(QString fileName);
    Q_INVOKABLE void clearAll();

    Q_INVOKABLE void updateBuffer(const QString &buffer);
    Q_INVOKABLE void executeCurrentCommand();
    Q_INVOKABLE QString handleAutoComplete();

public slots:
    void onTopicHovered(uint32_t id);
    void onTopicUnHovered(uint32_t id);
    void onTopicSelected(uint32_t id);
    void onTopicUnSelected(uint32_t id);

private:
    void synchGraphView();
    QString autoCompleteCommand(const QString &buffer, const QStringList &parts);
    QString autoCompleteTopics(const QStringList &parts, const QString &prefix);
    QString processAutocomplete(const QString &prefix,
                                const QStringList &matches,
                                const QStringList &parts);
    QString buildNewBuffer(const QString &replacement, const QStringList &parts);


private:
    TopicGraph m_graph;
    LayoutEngine m_layout;
    EvidenceDB m_evidenceDb;

    TopicGraphRepository m_repo;
    HeatScoreSystem *m_heatScore;
    TopicListModel *m_topicList;
    NodeListModel *m_nodeList;
    EdgeListModel *m_edgeList;
    QString m_currentBuffer;
    QString m_lastPrefix;
    QStringList m_lastMatches;
    int m_cycleIndex = -1;
};