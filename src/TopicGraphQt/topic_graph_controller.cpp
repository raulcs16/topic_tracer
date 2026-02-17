#include "fermatspiral_strategy.hpp"
#include "fmmm_strategy.hpp"
#include "graph_keys.hpp"
#include "orthogonal_strategy.hpp"
#include "path_analyzer.hpp"
#include "sugiyama_strategy.hpp"
#include "topic_graph_controller.hpp"
#include <QClipboard>
#include <QTimer>
#include <QtGui/qguiapplication.h>

TopicGraphController::TopicGraphController(QObject *parent)
    : QObject{parent}, m_graph{new TopicGraph()},
      m_repo{new TopicGraphRepository("./data")}, m_layout{new LayoutEngine()},
      m_tgstore{new TGStore()}, m_topicList{new TopicListModel(m_tgstore, this)},
      m_nodeList{new NodeListModel(m_tgstore, this)},
      m_edgeList(new EdgeListModel(m_tgstore, this)), m_rectList(new RectListModel(this)),
      m_evidenceDb{new EvidenceDB()},
      m_heatScore(new HeatScoreSystem(*m_evidenceDb, *m_graph)) {

    if (!m_graph || !m_repo || !m_layout || !m_tgstore || !m_topicList || !m_nodeList ||
        !m_edgeList)
        return;
    m_graph->addObserver(m_tgstore);
    m_graph->addObserver(m_layout);
    m_layout->addObserver(m_nodeList);
    m_layout->addObserver(m_edgeList);
    m_layout->addObserver(m_rectList);
    //----TG Store Singals BEGIN----
    connect(m_tgstore,
            &TGStore::labelUpdated,
            m_nodeList,
            &NodeListModel::onLabelUpdated);
    connect(m_tgstore,
            &TGStore::labelUpdated,
            m_topicList,
            &TopicListModel::onLabelUpdated);
    connect(m_tgstore,
            &TGStore::topicFlagUpdated,
            m_topicList,
            &TopicListModel::onFlagUpdated);
    connect(m_tgstore,
            &TGStore::topicFlagUpdated,
            m_nodeList,
            &NodeListModel::onFlagsUpdated);
    connect(m_tgstore,
            &TGStore::edgeFlagUpdated,
            m_edgeList,
            &EdgeListModel::onFlagUpdated);
    connect(m_tgstore, &TGStore::clear, m_topicList, &TopicListModel::onClear);

    //----TG Store Singals END----
    //----TopicListModel Singals Begin----
    connect(m_topicList,
            &TopicListModel::hoverRequested,
            this,
            &TopicGraphController::onTopicHoverRequested);

    connect(m_topicList,
            &TopicListModel::selectRequested,
            this,
            &TopicGraphController::onTopicSelectedRequested);

    connect(m_topicList,
            &TopicListModel::toggleSelectionRequest,
            this,
            &TopicGraphController::onTopicToggleSelectionRequest);
    connect(m_topicList,
            &TopicListModel::rangeSelectionRequest,
            this,
            &TopicGraphController::onTopicRangeSelectionRequest);

    //----TopicListModel Singals End----
    //----NodeListModel Singals Begin----
    connect(m_nodeList,
            &NodeListModel::selectRequested,
            this,
            &TopicGraphController::onTopicSelectedRequested);

    connect(m_nodeList,
            &NodeListModel::toggleSelectionRequest,
            this,
            &TopicGraphController::onTopicToggleSelectionRequest);
}
TopicGraphController::~TopicGraphController() {
    clearAll();

    m_graph->removeObserver(m_tgstore);
    m_graph->removeObserver(m_layout);
    delete m_repo;
    delete m_tgstore;
    delete m_graph;
    delete m_layout;
    delete m_evidenceDb;
    delete m_heatScore;
}

void TopicGraphController::createTopic(const QString &name) {
    if (m_graph) {
        m_graph->addTopic(name.toStdString());
    }
}
void TopicGraphController::onTopicHoverRequested(uint32_t id, bool isHovered) {

    m_tgstore->setTopicState(id, StateFlag::Hovered, isHovered);

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
void TopicGraphController::clearSelection() {
    if (!m_tgstore)
        return;
    for (auto i : m_selectedIds) {
        m_tgstore->setTopicState(i, StateFlag::Selected, false);
    }
    m_selectedIds.clear();
    m_lastSelectedId = -1;
    m_rangeSelectedId = -1;
}
//left click
void TopicGraphController::onTopicSelectedRequested(uint32_t id) {
    bool isTogglingOff = m_lastSelectedId == id;
    clearSelection();
    if (!isTogglingOff) {
        m_tgstore->setTopicState(id, StateFlag::Selected, true);
        m_selectedIds.push_back(id);
        m_lastSelectedId = id;
    }
}
//cmd click
void TopicGraphController::onTopicToggleSelectionRequest(uint32_t id) {
    auto it = std::find(m_selectedIds.begin(), m_selectedIds.end(), id);
    if (it == m_selectedIds.end()) {
        m_tgstore->setTopicState(id, StateFlag::Selected, true);
        m_selectedIds.push_back(id);
        m_lastSelectedId = id;
    } else {
        m_tgstore->setTopicState(id, StateFlag::Selected, false);
        m_selectedIds.erase(it);
    }
}
void TopicGraphController::onTopicRangeSelectionRequest(uint32_t id) {
    if (!m_tgstore || !m_topicList)
        return;
    if (m_lastSelectedId < 0)
        return;
    auto ids = m_topicList->getIdInRange(m_lastSelectedId, id);
    clearSelection();
    for (const auto id : ids) {
        m_tgstore->setTopicState(id, StateFlag::Selected, true);
        m_selectedIds.push_back(id);
    }
    m_lastSelectedId = id;
}
void TopicGraphController::onTopicRequested(const QString &name) {
    if (m_graph)
        m_graph->addTopic(name.toStdString());
}
void TopicGraphController::deleteTopic(const QString &topic) {
    if (m_graph)
        m_graph->deleteTopic(topic.toStdString());
}
void TopicGraphController::rename(const QString &topic, const QString &new_name) {
    m_graph->renameTopic(topic.toStdString(), new_name.toStdString());
}

void TopicGraphController::join(const QString &topicA,
                                const QString &topicB,
                                EdgeType type) {
    if (m_graph)
        m_graph->addEdge(topicA.toStdString(), topicB.toStdString(), type);
}
void TopicGraphController::noJoin(const QString &topicA, const QString &topicB) {
    if (m_graph)
        m_graph->removeEdge(topicA.toStdString(), topicB.toStdString());
}


void TopicGraphController::path(const QString &topicA, const QString &topicB) {
    if (!m_graph)
        return;
    auto ta = m_graph->getTopic(topicA.toStdString());
    auto tb = m_graph->getTopic(topicB.toStdString());
    if (ta == nullptr || tb == nullptr) {
        return;
    }
    auto parents = TG::PathAnalyzer::dijsktras(*m_graph, ta->id, tb->id);
    auto topicIds = TG::PathAnalyzer::topicPath(parents, tb->id);

    std::unordered_set<int> topicSet(topicIds.begin(), topicIds.end());
    for (const auto &topic : m_graph->topics()) {
        if (topicSet.contains(topic->id)) {
            m_tgstore->setTopicState(topic->id, StateFlag::InPath, true);
            m_tgstore->setTopicState(topic->id, StateFlag::Hidden, false);
        } else {
            m_tgstore->setTopicState(topic->id, StateFlag::InPath, false);
            m_tgstore->setTopicState(topic->id, StateFlag::Hidden, true);
        }
    }


    auto edgeKeys = TG::PathAnalyzer::edgePath(topicIds);
    std::unordered_set<std::string> edgeSet(edgeKeys.begin(), edgeKeys.end());
    for (const auto &edge : m_graph->edges()) {
        if (edgeSet.contains(edge->key)) {
            m_tgstore->setEdgeState(edge->key, StateFlag::InPath, true);
            m_tgstore->setEdgeState(edge->key, StateFlag::Hidden, false);
        } else {
            m_tgstore->setEdgeState(edge->key, StateFlag::InPath, false);
            m_tgstore->setEdgeState(edge->key, StateFlag::Hidden, true);
        }
    }
}
void TopicGraphController::noPath() {
    if (!m_graph || !m_tgstore)
        return;
    for (const auto topics : m_graph->topics()) {
        m_tgstore->setTopicState(topics->id, StateFlag::Hidden, false);
        m_tgstore->setTopicState(topics->id, StateFlag::InPath, false);
    }
    for (const auto edges : m_graph->edges()) {
        m_tgstore->setEdgeState(edges->key, StateFlag::Hidden, false);
        m_tgstore->setEdgeState(edges->key, StateFlag::InPath, false);
    }
}
void TopicGraphController::calculateHeatScores() {
    auto map = m_heatScore->computeAllHeatScores();
    for (const auto [topic, score] : map) {
        if (!score)
            continue;
        m_nodeList->updateHeatScore(topic->id, score);
    }
}
void TopicGraphController::save(QString fileName) {
    if (!m_repo)
        return;
    bool saved = m_repo->save(*m_graph, fileName);
}
void TopicGraphController::load(QString fileName) {
    if (!m_repo || !m_graph)
        return;
    m_graph->clear();
    m_graph->beginBatchLoad();
    bool load = m_repo->load(*m_graph, fileName);
    m_graph->endBatchLoad();
    calculateHeatScores();
    m_rectList->setSceneBounds(m_layout->getGlobalBoundingBox());
}
void TopicGraphController::clearAll() { m_graph->clear(); }


void TopicGraphController::executeCommand(QString raw_cmd) {
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
            auto topic = m_graph->getTopic(arg.toStdString());
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
            while (parts.length()) {
                createTopic(parts.takeFirst());
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

QString TopicGraphController::getAutoComplete(QString raw_cmd) {
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
    QString match = m_tgstore->findMatch(partial);
    if (!match.isEmpty()) {
        parts.append(match);
        return parts.join(" ");
    }
    //else
    // QString cmd = parts.takeFirst().toLower();
    // if (cmd == "join") {
    //     QString partial = parts.takeLast();
    //     QString match = m_tgstore->findMatch(partial);
    //     if (!match.isEmpty()) {
    //         parts.append(match);
    //         return cmd + " " + parts.join(" ");
    //     }
    // } else if (cmd == "no") {
    //     if (parts.length() == 1) {
    //         QString partial = parts.takeFirst();
    //         for (auto &c : commands)
    //             if (c.startsWith(partial))
    //                 return cmd + " " + c;
    //     }
    //     QString cmd2 = parts.takeFirst().toLower();
    //     if (cmd2 == "join") {
    //         QString partial = parts.takeLast();
    //         QString match = m_tgstore->findMatch(partial);
    //         if (!match.isEmpty()) {
    //             parts.append(match);
    //             return cmd + " " + cmd2 + " " + parts.join(" ");
    //         }
    //     }
    // }
    return raw_cmd;
}

void TopicGraphController::copySelection() {
    QClipboard *clipboard = QGuiApplication::clipboard();
    QStringList list;
    for (const auto id : m_selectedIds) {
        auto label = m_tgstore->label(id);
        if (!label.isEmpty()) {
            list.push_back(label);
        }
    }
    clipboard->setText(list.join("\n"));
}
void TopicGraphController::selectAll() {
    for (const auto &topic : m_graph->topics()) {
        m_tgstore->setTopicState(topic->id, StateFlag::Selected, true);
        m_selectedIds.push_back(topic->id);
    }
}