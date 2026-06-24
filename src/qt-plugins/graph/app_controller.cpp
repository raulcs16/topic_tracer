#include "app_controller.hpp"
#include "file_manager_dev.hpp"
#include "file_manager_qt.hpp"
#include <QCoreApplication>
#include <QTimer>

AppController::AppController(QObject *parent) : QObject{parent} {
    // 1. Resolve implementation based on Build Mode
    // #ifdef QT_DEBUG
    //     // Uses the local project folder during development
    //     m_fileManager = new FileManagerDev("./data");
    // #else
    //     // Uses ~/Library/Application Support/TopicTracer or equivalent
    //     m_fileManager = new FileManagerQt(QCoreApplication::applicationName());
    // #endif
    // m_stoe = new GraphStore(this);
    // m_uiContext = new UIContext(m_store, this);
    // 2. Initialize remaining systems
    // m_graph = new Graph()
    // m_nodeReg = std::make_shared<NodeTypeRegistry>();
    // m_edgeReg = std::make_shared<NodeTypeRegistry>();
    // m_pgraph = new PGraph(m_nodeReg, m_edgeReg);
    // m_layout = new LayoutEngine();
    // m_nodeTypePresentor = new NodeTypePresentor(m_nodeReg, m_layout, this);
    // m_bboxes = new BoundingBoxListModel(m_nodeTypePresentor, this);
    // m_terminal = new TerminalListModel(this);
    // m_evidenceDb = new EvidenceDB();
    // m_heatScore = new HeatScoreSystem(*m_evidenceDb, *m_graph);
    // m_repo = new GraphRepository(m_fileManager);

    // CommandContext now receives the IFileManager interface seamlessly
    // m_commandFactory = new CommandFactory(
    //     new CommandContext{m_graph, m_repo, m_layout, m_uiContext, m_fileManager});

    // 3. Setup Observer Chain
    // if (!m_graph || !m_repo || !m_layout || !m_uiContext || !m_pgraph)
    //     return;

    // m_graph->addObserver(m_uiContext->store());
    // m_graph->addObserver(m_layout);
    // m_layout->addObserver(m_uiContext->store());

    // connect(m_uiContext,
    //         &UIContext::modeChanged,
    //         this,
    //         &AppController::calculateHeatScores);
}
AppController::~AppController() {
    // delete m_commandFactory;
    // delete m_repo;
    // delete m_graph;
    // delete m_evidenceDb;
    // delete m_heatScore;
    // delete m_fileManager; // Don't forget this one!
    // delete m_layout;
    // delete m_pgraph;
    // m_nodeReg.release();
    // m_nodeReg.release();
}


void AppController::onTopicRequested(const QString &name) {
    // if (m_graph)
    //     m_graph->addNode(name.toStdString());
}

void AppController::calculateHeatScores() {
    // auto map = m_uiContext->mode() == UIContext::ViewMode::Progress
    //                ? m_heatScore->computeProgressScores()
    //                : m_heatScore->computeStressScores();

    // for (const auto [id, score] : map) {
    //     m_uiContext->store()->setNodeHeat(id, score);
    // }
}


void AppController::executeCommand(const QString &raw_cmd) {
    QStringList parts = raw_cmd.split(" ", Qt::SkipEmptyParts);
    m_uiContext->terminalListModel()->addEntry(raw_cmd, EntryType::Command);
    // 2. Try to create the command
    // auto command = m_commandFactory->create(parts);
    // if (!command) {
    //     m_uiContext->terminalListModel()->addEntry("Unknown command: " + parts.first(),
    //                                                EntryType::Error);
    //     return;
    // }

    // // 3. Execute and handle results
    // CommandResult result = command->execute();

    // if (!result.message.isEmpty()) {
    //     m_uiContext->terminalListModel()->addEntry(result.message, result.type);
    // }

    // if (result.success) {
    //     // m_history.push(std::move(command)); // For undo/redo later
    // }
}

void AppController::handleSuggestion(const QString &input) {
    // QList<AutoSuggestion> matches = m_commandFactory->findMatch(input);
    // if (matches.size() == 0) {
    //     return;
    // }
    // if (matches.size() == 1) {
    //     const auto &match = matches.first();
    //     QString fullSuggestion = input.left(match.startIndex) + match.suggestion;
    //     emit suggestionReady(fullSuggestion);
    // } else {
    //     QStringList names;
    //     for (const auto &m : matches) {
    //         names << m.suggestion;
    //     }

    //     QString options = names.join("\t");
    //     m_uiContext->terminalListModel()->addEntry(options, EntryType::Hint);
    // }
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
