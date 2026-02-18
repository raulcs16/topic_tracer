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

AppController::AppController(QObject *parent)
    : QObject{parent}, m_graph{new Graph()}, m_repo{new GraphRepository("./data")},
      m_layout{new LayoutEngine()}, m_store{new GraphStore()},
      m_labelList{new LabelListModel(m_store, this)},
      m_nodeList{new NodeListModel(m_store, this)},
      m_edgeList(new EdgeListModel(m_store, this)), m_rectList(new RectListModel(this)),
      m_evidenceDb{new EvidenceDB()},
      m_heatScore(new HeatScoreSystem(*m_evidenceDb, *m_graph)) {

    if (!m_graph || !m_repo || !m_layout || !m_store || !m_labelList || !m_nodeList ||
        !m_edgeList)
        return;
    m_graph->addObserver(m_store);
    m_graph->addObserver(m_layout);
    m_layout->addObserver(m_nodeList);
    m_layout->addObserver(m_edgeList);
    m_layout->addObserver(m_rectList);
    //----TG Store Singals BEGIN----
    connect(m_store,
            &GraphStore::labelUpdated,
            m_nodeList,
            &NodeListModel::onLabelUpdated);
    connect(m_store,
            &GraphStore::labelUpdated,
            m_labelList,
            &LabelListModel::onLabelUpdated);
    connect(m_store,
            &GraphStore::nodeFlagUpdated,
            m_labelList,
            &LabelListModel::onFlagUpdated);
    connect(m_store,
            &GraphStore::nodeFlagUpdated,
            m_nodeList,
            &NodeListModel::onFlagsUpdated);
    connect(m_store,
            &GraphStore::edgeFlagUpdated,
            m_edgeList,
            &EdgeListModel::onFlagUpdated);
    connect(m_store, &GraphStore::clear, m_labelList, &LabelListModel::onClear);

    //----TG Store Singals END----
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
    clearAll();

    m_graph->removeObserver(m_store);
    m_graph->removeObserver(m_layout);
    delete m_repo;
    delete m_store;
    delete m_graph;
    delete m_layout;
    delete m_evidenceDb;
    delete m_heatScore;
}

void AppController::createTopic(const QString &name) {
    m_graph->addNode(name.toStdString());
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
void AppController::deleteTopic(const QString &topic) {
    if (m_graph)
        m_graph->deleteNode(topic.toStdString());
}
void AppController::rename(const QString &topic, const QString &new_name) {
    m_graph->renameNode(topic.toStdString(), new_name.toStdString());
}

void AppController::join(const QString &topicA, const QString &topicB, EdgeType type) {
    if (m_graph)
        m_graph->addEdge(topicA.toStdString(), topicB.toStdString(), type);
}
void AppController::noJoin(const QString &topicA, const QString &topicB) {
    if (m_graph)
        m_graph->removeEdge(topicA.toStdString(), topicB.toStdString());
}


void AppController::path(const QString &topicA, const QString &topicB) {
    if (!m_graph)
        return;
    auto ta = m_graph->getNode(topicA.toStdString());
    auto tb = m_graph->getNode(topicB.toStdString());
    if (ta == nullptr || tb == nullptr) {
        return;
    }
    auto parents = TG::PathAnalyzer::dijsktras(*m_graph, ta->id, tb->id);
    auto topicIds = TG::PathAnalyzer::topicPath(parents, tb->id);

    std::unordered_set<int> topicSet(topicIds.begin(), topicIds.end());
    for (const auto &topic : m_graph->nodes()) {
        if (topicSet.contains(topic->id)) {
            m_store->setNodeState(topic->id, StateFlag::InPath, true);
            m_store->setNodeState(topic->id, StateFlag::Hidden, false);
        } else {
            m_store->setNodeState(topic->id, StateFlag::InPath, false);
            m_store->setNodeState(topic->id, StateFlag::Hidden, true);
        }
    }


    auto edgeKeys = TG::PathAnalyzer::edgePath(topicIds);
    std::unordered_set<std::string> edgeSet(edgeKeys.begin(), edgeKeys.end());
    for (const auto &edge : m_graph->edges()) {
        if (edgeSet.contains(edge->key)) {
            m_store->setEdgeState(edge->key, StateFlag::InPath, true);
            m_store->setEdgeState(edge->key, StateFlag::Hidden, false);
        } else {
            m_store->setEdgeState(edge->key, StateFlag::InPath, false);
            m_store->setEdgeState(edge->key, StateFlag::Hidden, true);
        }
    }
}
void AppController::noPath() {
    if (!m_graph || !m_store)
        return;
    for (const auto topics : m_graph->nodes()) {
        m_store->setNodeState(topics->id, StateFlag::Hidden, false);
        m_store->setNodeState(topics->id, StateFlag::InPath, false);
    }
    for (const auto edges : m_graph->edges()) {
        m_store->setEdgeState(edges->key, StateFlag::Hidden, false);
        m_store->setEdgeState(edges->key, StateFlag::InPath, false);
    }
}
void AppController::calculateHeatScores() {
    auto map = m_heatScore->computeAllHeatScores();
    for (const auto [topic, score] : map) {
        if (!score)
            continue;
        m_nodeList->updateHeatScore(topic->id, score);
    }
}
void AppController::save(QString fileName) {
    if (!m_repo)
        return;
    bool saved = m_repo->save(*m_graph, fileName);
}
void AppController::load(QString fileName) {
    if (!m_repo || !m_graph)
        return;
    m_graph->clear();
    m_graph->beginBatchLoad();
    bool load = m_repo->load(*m_graph, fileName);
    m_graph->endBatchLoad();
    calculateHeatScores();
    m_rectList->setSceneBounds(m_layout->getGlobalBoundingBox());
}
void AppController::clearAll() { m_graph->clear(); }


void AppController::executeCommand(QString raw_cmd) {
    raw_cmd = raw_cmd.trimmed();
    QStringList parts = raw_cmd.split(" ", Qt::SkipEmptyParts);
    if (parts.empty())
        return;
    QString cmd = parts.takeFirst().toLower();
    if (cmd == "clear") {
        clearAll();
    }
    //if remaining parts
    if (parts.length() == 1) {
        QString arg = parts.takeFirst();
        if (cmd == "save") {
            save(arg);
        } else if (cmd == "load") {
            load(arg);
        } else if (cmd == "rm") {
            deleteTopic(arg);
        } else if (cmd == "touch") {
            createTopic(arg);
        } else if (cmd == "focus") {
            auto topic = m_graph->getNode(arg.toStdString());
            if (topic != nullptr) {
                auto clusterId = m_layout->getNodeBoundingBox(topic->id);
                m_rectList->setSceneBounds(clusterId);
            }
        } else if (cmd == "no") {
            if (arg == "path") {
                noPath();
            } else if (arg == "focus") {
                m_rectList->setSceneBounds(m_layout->getGlobalBoundingBox());
            }
        }
    } else if (parts.length() == 2) {
        QString arg1 = parts.takeFirst();
        QString arg2 = parts.takeFirst();
        if (cmd == "path") {
            path(arg1, arg2);
        } else if (cmd == "mv") {
            rename(arg1, arg2);
        } else if (cmd == "join") {
            join(arg1, arg2);
        } else if (cmd == "touch") {
            createTopic(arg1);
            createTopic(arg2);
        }
    } else if (parts.length() > 2) {
        if (cmd == "touch") {
            QString last = parts.takeLast();
            QString preLast = parts.takeLast();
            bool joinLast = preLast == ">";
            if (!joinLast) {
                parts.append(preLast);
                parts.append(last);
            }
            while (parts.length()) {
                auto arg = parts.takeFirst();
                createTopic(arg);
                if (joinLast) {
                    join(last, arg);
                }
            }
        } else if (cmd == "no") {
            QString nextCmd = parts.takeFirst();
            if (nextCmd == "join") {
                if (parts.length() != 2)
                    return;
                QString arg1 = parts.takeFirst();
                QString arg2 = parts.takeFirst();
                noJoin(arg1, arg2);
            }
        }
    }
}

QString AppController::getAutoComplete(QString raw_cmd) {
    raw_cmd = raw_cmd.trimmed();
    QStringList parts = raw_cmd.split(" ", Qt::SkipEmptyParts);
    static QStringList commands =
        {"clear", "save", "load", "touch", "join", "rm", "path", "mv", "focus"};
    if (parts.empty())
        return raw_cmd;
    //autocomplete suggests a cmd
    if (parts.size() == 1) {
        QString partial = parts.takeFirst();
        for (auto &cmd : commands)
            if (cmd.startsWith(partial))
                return cmd; //TODO: return all matches
        return raw_cmd;
    }
    QString partial = parts.takeLast();
    QString match = m_store->findMatch(partial);
    if (!match.isEmpty()) {
        parts.append(match);
        return parts.join(" ");
    }

    return raw_cmd;
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