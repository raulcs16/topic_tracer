#include "app_controller.hpp"
#include "graph_keys.hpp"
#include <QTimer>


AppController::AppController(UIContext *ui, QObject *parent)
    : QObject{parent}, m_uiContext{ui}, m_graph{new Graph()},
      m_repo{new GraphRepository("./data")}, m_layout{new LayoutEngine()},
      m_evidenceDb{new EvidenceDB()},
      m_heatScore(new HeatScoreSystem(*m_evidenceDb, *m_graph)),
      m_commandFactory(
          new CommandFactory(new CommandContext{m_graph, m_repo, m_layout, m_uiContext}))

{
    if (!m_graph || !m_repo || !m_layout || !m_uiContext)
        return;
    m_graph->addObserver(m_uiContext->store());
    m_graph->addObserver(m_layout);
    m_layout->addObserver(m_uiContext->store());

    connect(m_uiContext,
            &UIContext::modeChanged,
            this,
            &AppController::calculateHeatScores);
}
AppController::~AppController() {
    delete m_repo;
    delete m_layout;
    delete m_graph;
    delete m_evidenceDb;
    delete m_heatScore;
    delete m_commandFactory;
}


void AppController::onTopicRequested(const QString &name) {
    if (m_graph)
        m_graph->addNode(name.toStdString());
}

void AppController::calculateHeatScores() {
    auto map = m_uiContext->mode() == UIContext::ViewMode::Progress
                   ? m_heatScore->computeProgressScores()
                   : m_heatScore->computeStressScores();

    for (const auto [id, score] : map) {
        m_uiContext->store()->setNodeHeat(id, score);
    }
}


void AppController::executeCommand(QString raw_cmd) {
    raw_cmd = raw_cmd.trimmed();
    QStringList parts = raw_cmd.split(" ", Qt::SkipEmptyParts);
    m_uiContext->terminalListModel()->addEntry(raw_cmd, EntryType::Command);
    // 2. Try to create the command
    auto command = m_commandFactory->create(parts);
    if (!command) {
        m_uiContext->terminalListModel()->addEntry("Unknown command: " + parts.first(),
                                                   EntryType::Error);
        return;
    }

    // 3. Execute and handle results
    CommandResult result = command->execute();

    if (!result.message.isEmpty()) {
        m_uiContext->terminalListModel()->addEntry(result.message, result.type);
    }

    if (result.success) {
        // m_history.push(std::move(command)); // For undo/redo later
    }
}
