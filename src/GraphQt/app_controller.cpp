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

AppController::AppController(GraphStore *store, QObject *parent)
    : QObject{parent}, m_graph{new Graph()}, m_repo{new GraphRepository("./data")},
      m_layout{new LayoutEngine()}, m_store{store},
      m_labelList{new LabelListModel(m_store, this)},
      m_nodeList{new NodeListModel(m_store, this)},
      m_edgeList(new EdgeListModel(m_store, this)), m_rectList(new RectListModel(this)),
      m_evidenceDb{new EvidenceDB()},
      m_heatScore(new HeatScoreSystem(*m_evidenceDb, *m_graph)),
      m_commandFactory(new CommandFactory(
          new CommandContext{m_graph, m_repo, m_layout, m_rectList, m_store}))

{
    m_mode = AppMode::Progress;
    if (!m_graph || !m_repo || !m_layout || !m_store || !m_labelList || !m_nodeList ||
        !m_edgeList)
        return;
    m_graph->addObserver(m_store);
    m_graph->addObserver(m_layout);
    m_layout->addObserver(m_nodeList);
    m_layout->addObserver(m_edgeList);
    m_layout->addObserver(m_rectList);
    //----LabelListModel Singals Begin----
    connect(m_labelList,
            &LabelListModel::hoverRequested,
            this,
            &AppController::onTopicHoverRequested);

    connect(m_labelList,
            &LabelListModel::selectRequested,
            this,
            &AppController::onTopicSelectedRequested);

    connect(m_labelList,
            &LabelListModel::toggleSelectionRequest,
            this,
            &AppController::onTopicToggleSelectionRequest);
    connect(m_labelList,
            &LabelListModel::rangeSelectionRequest,
            this,
            &AppController::onTopicRangeSelectionRequest);

    //----LabelListModel Singals End----
    //----NodeListModel Singals Begin----
    connect(m_nodeList,
            &NodeListModel::selectRequested,
            this,
            &AppController::onTopicSelectedRequested);

    connect(m_nodeList,
            &NodeListModel::toggleSelectionRequest,
            this,
            &AppController::onTopicToggleSelectionRequest);
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
    m_rectList->setSceneBounds(boundingBoxId);
}
void AppController::clearSelection() {
    if (!m_store)
        return;
    for (auto i : m_selectedIds) {
        m_store->setNodeState(i, StateFlag::Selected, false);
    }
    m_selectedIds.clear();
    m_lastSelectedId = -1;
    m_rangeSelectedId = -1;
}
//left click
void AppController::onTopicSelectedRequested(uint32_t id) {
    bool isTogglingOff = m_lastSelectedId == id;
    clearSelection();
    if (!isTogglingOff) {
        m_store->setNodeState(id, StateFlag::Selected, true);
        m_selectedIds.push_back(id);
        m_lastSelectedId = id;
    }
}
//cmd click
void AppController::onTopicToggleSelectionRequest(uint32_t id) {
    auto it = std::find(m_selectedIds.begin(), m_selectedIds.end(), id);
    if (it == m_selectedIds.end()) {
        m_store->setNodeState(id, StateFlag::Selected, true);
        m_selectedIds.push_back(id);
        m_lastSelectedId = id;
    } else {
        m_store->setNodeState(id, StateFlag::Selected, false);
        m_selectedIds.erase(it);
    }
}
void AppController::onTopicRangeSelectionRequest(uint32_t id) {
    if (!m_store || !m_labelList)
        return;
    if (m_lastSelectedId < 0)
        return;
    auto ids = m_labelList->getIdInRange(m_lastSelectedId, id);
    clearSelection();
    for (const auto id : ids) {
        m_store->setNodeState(id, StateFlag::Selected, true);
        m_selectedIds.push_back(id);
    }
    m_lastSelectedId = id;
}
void AppController::onTopicRequested(const QString &name) {
    if (m_graph)
        m_graph->addNode(name.toStdString());
}

void AppController::calculateHeatScores() {
    auto map = m_mode == AppMode::Progress ? m_heatScore->computeProgressScores()
                                           : m_heatScore->computeStressScores();

    for (const auto [id, score] : map) {
        m_nodeList->updateHeatScore(id, score);
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
    for (const auto id : m_selectedIds) {
        auto label = m_store->label(id);
        if (!label.isEmpty()) {
            list.push_back(label);
        }
    }
    clipboard->setText(list.join("\n"));
}
void AppController::selectAll() {
    for (const auto &topic : m_graph->nodes()) {
        m_store->setNodeState(topic->id, StateFlag::Selected, true);
        m_selectedIds.push_back(topic->id);
    }
}

void AppController::setMode(AppMode mode) {
    if (m_mode == mode)
        return;
    m_mode = mode;
    calculateHeatScores();
    emit appModeChanged();
}