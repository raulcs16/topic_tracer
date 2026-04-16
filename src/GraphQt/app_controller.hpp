#pragma once

#include "command_factory.hpp"
#include "evidence.hpp"
#include "graph.hpp"
#include "graph_repo.hpp"
#include "heat_score.hpp"
#include "ifile_manager.hpp"
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
public:
    explicit AppController(UIContext *ui, QObject *parent = nullptr);
    ~AppController();
    UIContext *uiContext() const { return m_uiContext; }

    Q_INVOKABLE void executeCommand(const QString &raw_cmd);
    Q_INVOKABLE void handleSuggestion(const QString &input);
    Q_INVOKABLE void handleInput(const QString &input);

signals:
    void suggestionReady(const QString &suggestion);
public slots:
    void calculateHeatScores();
    void onTopicRequested(const QString &topic);

private:
    Graph *m_graph;
    LayoutEngine *m_layout;
    IFileManager *m_fileManager;
    GraphRepository *m_repo;
    UIContext *m_uiContext;
    CommandFactory *m_commandFactory;
    EvidenceDB *m_evidenceDb;
    HeatScoreSystem *m_heatScore;
};
