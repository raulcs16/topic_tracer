#include "app_controller.hpp"

#include "fermatspiral_strategy.hpp"
#include "fmmm_strategy.hpp"
#include "graph_keys.hpp"

#include "orthogonal_strategy.hpp"
#include "path_analyzer.hpp"
#include "sugiyama_strategy.hpp"
#include <QClipboard>
#include <QTimer>
#include <QtGui/qguiapplication.h>

AppController::AppController(GraphStore *store, UIContext *ui, QObject *parent)
    : QObject{parent}, m_store(store), m_uiContext{ui}, m_graph{new Graph()},
      m_repo{new GraphRepository("./data")}, m_layout{new LayoutEngine()},
      m_evidenceDb{new EvidenceDB()},
      m_heatScore(new HeatScoreSystem(*m_evidenceDb, *m_graph)),
      m_commandFactory(new CommandFactory(new CommandContext{m_graph,
                                                             m_repo,
                                                             m_layout,
                                                             m_uiContext->rectListModel(),
                                                             m_store}))

{
    m_mode = AppMode::Progress;
    if (!m_graph || !m_repo || !m_layout || !m_store || !m_uiContext)
        return;
    m_graph->addObserver(m_store);
    m_graph->addObserver(m_layout);
    m_layout->addObserver(m_uiContext->nodeListModel());
    m_layout->addObserver(m_uiContext->edgeListModel());
    m_layout->addObserver(m_uiContext->rectListModel());
}
AppController::~AppController() {
    m_graph->removeObserver(m_store);
    m_graph->removeObserver(m_layout);
    delete m_repo;
    delete m_graph;
    delete m_layout;
    delete m_evidenceDb;
    delete m_heatScore;
}

void AppController::onTopicHoverRequested(uint32_t id, bool isHovered) {
    m_store->setNodeState(id, StateFlag::Hovered, isHovered);

    if (isHovered) {
        m_hoveredId = id;
    } else if (m_hoveredId == id) {
        m_hoveredId = -1;
    } else {
        return;
    }
    auto boundingBoxId = m_hoveredId == id ? m_layout->getNodeBoundingBox(id)
                                           : m_layout->getGlobalBoundingBox();
    m_uiContext->rectListModel()->setSceneBounds(boundingBoxId);
}

void AppController::onTopicRequested(const QString &name) {
    if (m_graph)
        m_graph->addNode(name.toStdString());
}

void AppController::calculateHeatScores() {
    auto map = m_mode == AppMode::Progress ? m_heatScore->computeProgressScores()
                                           : m_heatScore->computeStressScores();

    for (const auto [id, score] : map) {
        m_uiContext->nodeListModel()->updateHeatScore(id, score);
    }
}


void AppController::executeCommand(QString raw_cmd) {
    raw_cmd = raw_cmd.trimmed();
    QStringList parts = raw_cmd.split(" ", Qt::SkipEmptyParts);
    auto command = m_commandFactory->create(parts);
    if (command) {
        command->execute();
    }
    //     } else if (cmd == "mode") {
    //         if (arg.toLower() == "progress") {
    //             setMode(AppMode::Progress);
    //         } else if (arg.toLower() == "stress") {
    //             setMode(AppMode::Stress);
    //         }   //     }
}


void AppController::copySelection() {
    QClipboard *clipboard = QGuiApplication::clipboard();
    QStringList list;
    for (const auto id : m_uiContext->selectionManager()->selectedIds()) {
        auto label = m_store->label(id);
        if (!label.isEmpty()) {
            list.push_back(label);
        }
    }
    clipboard->setText(list.join("\n"));
}
void AppController::selectAll() { m_uiContext->selectionManager()->selectAll(); }

void AppController::setMode(AppMode mode) {
    if (m_mode == mode)
        return;
    m_mode = mode;
    calculateHeatScores();
    emit appModeChanged();
}