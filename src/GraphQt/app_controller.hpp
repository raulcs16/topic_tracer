#pragma once

#include "command_factory.hpp"
#include "evidence.hpp"
#include "graph.hpp"
#include "graph_repo.hpp"
#include "heat_score.hpp"
#include "layout_engine.hpp"
#include "selection_manager.hpp"
#include "ui_context.hpp"
#include "ui_states.hpp"
#include <QObject>
#include <QtQml/qqml.h>

class AppController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Created in main.cpp")
    Q_PROPERTY(UIContext *uiContext READ uiContext CONSTANT)
    Q_PROPERTY(AppMode appMode READ appMode NOTIFY appModeChanged)
public:
    enum class AppMode {
        Progress,
        Stress
    };
    Q_ENUM(AppMode)
    explicit AppController(GraphStore *store, UIContext *ui, QObject *parent = nullptr);
    ~AppController();
    AppMode appMode() const { return m_mode; }
    UIContext *uiContext() const { return m_uiContext; }

    void calculateHeatScores();
    Q_INVOKABLE void executeCommand(QString raw_cmd);

    Q_INVOKABLE void copySelection();
    Q_INVOKABLE void selectAll();


public slots:
    void onTopicRequested(const QString &topic);
    void onTopicHoverRequested(uint32_t id, bool isHovered);
signals:
    void appModeChanged();

private:
    void setMode(AppMode mode);

private:
    Graph *m_graph;
    LayoutEngine *m_layout;
    GraphStore *m_store;
    GraphRepository *m_repo;
    UIContext *m_uiContext;
    CommandFactory *m_commandFactory;

    EvidenceDB *m_evidenceDb;
    HeatScoreSystem *m_heatScore;

    int m_cycleIndex = -1;
    int m_hoveredId = -1;
    AppMode m_mode;
};
