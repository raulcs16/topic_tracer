#include "app_controller.hpp"
#include "file_manager_dev.hpp"
#include "graph_keys.hpp"
#include <QTimer>

AppController::AppController(UIContext *ui, QObject *parent)
    : QObject{parent}, m_uiContext{ui}, m_graph{new Graph()},
      m_fileManager{new FileManagerDev("./data")}, m_layout{new LayoutEngine()},
      m_evidenceDb{new EvidenceDB()},
      m_heatScore(new HeatScoreSystem(*m_evidenceDb, *m_graph)),
      m_repo(new GraphRepository{m_fileManager}),
      m_commandFactory(new CommandFactory(
          new CommandContext{m_graph, m_repo, m_layout, m_uiContext, m_fileManager}))

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


void AppController::executeCommand(const QString &raw_cmd) {
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

void AppController::handleSuggestion(const QString &input) {
    QList<AutoSuggestion> matches = m_commandFactory->findMatch(input);
    if (matches.size() == 0) {
        return;
    }
    if (matches.size() == 1) {
        const auto &match = matches.first();
        QString fullSuggestion = input.left(match.startIndex) + match.suggestion;
        emit suggestionReady(fullSuggestion);
    } else {
        QStringList names;
        for (const auto &m : matches) {
            names << m.suggestion;
        }

        QString options = names.join("\t");
        m_uiContext->terminalListModel()->addEntry(options, EntryType::Hint);
    }
}
void AppController::handleInput(const QString &input) {
    // if (input.endsWith("?")) {
    //     auto clean = input.left(input.length() - 1).trimmed();
    //     QStringList parts = clean.split(" ", Qt::SkipEmptyParts);
    //     auto command = m_commandFactory->create(parts);
    //     if (command != nullptr) {
    //         auto hint = command->getHint();
    //         if (hint.size() > 0) {
    //             m_uiContext->terminalListModel()->addEntry(input, EntryType::Command);
    //             m_uiContext->terminalListModel()->addEntry(hint, EntryType::Hint);
    //         }
    //     }
    // }
}
