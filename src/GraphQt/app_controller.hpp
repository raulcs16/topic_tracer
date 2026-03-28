#pragma once

#include "command_factory.hpp"
#include "edge_list_model.hpp"
#include "evidence.hpp"
#include "graph.hpp"
#include "graph_repo.hpp"
#include "heat_score.hpp"
#include "label_list_model.hpp"
#include "layout_engine.hpp"
#include "node_list_model.hpp"
#include "rect_list_model.hpp"
#include "ui_states.hpp"
#include <QObject>
#include <QtQml/qqml.h>

class AppController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(LabelListModel *labelListModel READ labelListModel CONSTANT)
    Q_PROPERTY(EdgeListModel *edgeListModel READ edgeListModel CONSTANT)
    Q_PROPERTY(NodeListModel *nodeListModel READ nodeListModel CONSTANT)
    Q_PROPERTY(RectListModel *rectListModel READ rectListModel CONSTANT)
    Q_PROPERTY(AppMode appMode READ appMode NOTIFY appModeChanged)

public:
    enum class AppMode {
        Progress,
        Stress
    };
    Q_ENUM(AppMode)
    explicit AppController(QObject *parent = nullptr);
    ~AppController();
    LabelListModel *labelListModel() const { return m_labelList; }
    NodeListModel *nodeListModel() const { return m_nodeList; }
    EdgeListModel *edgeListModel() const { return m_edgeList; }
    RectListModel *rectListModel() const { return m_rectList; }
    AppMode appMode() const { return m_mode; }

    void calculateHeatScores();

    //QML API
    Q_INVOKABLE void deleteTopic(const QString &name);
    Q_INVOKABLE void join(const QString &topicA,
                          const QString &topicB,
                          EdgeType type = EdgeType::Composes);
    Q_INVOKABLE void noJoin(const QString &topicA, const QString &topicB);
    Q_INVOKABLE void rename(const QString &topic, const QString &new_name);
    Q_INVOKABLE void path(const QString &topicA, const QString &topicB);
    Q_INVOKABLE void noPath();

    //ICommand
    Q_INVOKABLE void createTopic(const QString &name);
    Q_INVOKABLE void save(QString fileName);
    Q_INVOKABLE void load(QString fileName);
    Q_INVOKABLE void clearAll();

    Q_INVOKABLE void executeCommand(QString raw_cmd);
    Q_INVOKABLE QString getAutoComplete(QString raw_cmd);

    Q_INVOKABLE void copySelection();
    Q_INVOKABLE void selectAll();


public slots:
    void onTopicRequested(const QString &topic);
    void onTopicHoverRequested(uint32_t id, bool isHovered);
    void onTopicSelectedRequested(uint32_t id);
    void onTopicToggleSelectionRequest(uint32_t id);
    void onTopicRangeSelectionRequest(uint32_t id);
signals:
    void appModeChanged();

private:
    void clearSelection();
    void setMode(AppMode mode);

private:
    Graph *m_graph;
    LayoutEngine *m_layout;
    GraphStore *m_store;
    GraphRepository *m_repo;
    LabelListModel *m_labelList;
    NodeListModel *m_nodeList;
    EdgeListModel *m_edgeList;
    RectListModel *m_rectList;

    CommandFactory *m_commandFactory;

    EvidenceDB *m_evidenceDb;
    HeatScoreSystem *m_heatScore;

    int m_cycleIndex = -1;
    std::vector<uint32_t> m_selectedIds;
    int m_lastSelectedId = -1;
    int m_rangeSelectedId = -1;
    int m_hoveredId = -1;
    AppMode m_mode;
};
