#pragma once

// #include "boundingbox_list_model.hpp"
#include "layout_engine.hpp"
// #include "node_type_presentor.hpp"
#include "pgraph.hpp"
// #include "terminal_list_model.hpp"
#include <QObject>
#include <QtQml/qqml.h>

class AppController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    // QML_UNCREATABLE("Created in main.cpp")
    // Q_PROPERTY(BoundingBoxListModel *bboxListModel READ bboxListModel CONSTANT)
    // Q_PROPERTY(TerminalListModel *terminalListModel READ terminalListModel CONSTANT)

public:
    explicit AppController(QObject *parent = nullptr);
    ~AppController();
    // UIContext *uiContext() const { return m_uiContext; }

    // BoundingBoxListModel *bboxListModel() const { return m_bboxes; }
    // TerminalListModel *terminalListModel() const { return m_terminal; }
    Q_INVOKABLE void executeCommand(const QString &raw_cmd);
    Q_INVOKABLE void handleSuggestion(const QString &input);
    Q_INVOKABLE void handleInput(const QString &input);

signals:
    void suggestionReady(const QString &suggestion);
public slots:
    void calculateHeatScores();
    void onTopicRequested(const QString &topic);

private:
    // Graph *m_graph;
    // GraphStore *m_store;
    // NodeTypeRegPtr m_nodeReg;
    // EdgeTypeRegPtr m_edgeReg;
    // PGraph *m_pgraph;

    // LayoutEngine *m_layout;

    // NodeTypePresentor *m_nodeTypePresentor;

    // BoundingBoxListModel *m_bboxes;

    // TerminalListModel *m_terminal;
};
