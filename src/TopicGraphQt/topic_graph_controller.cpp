#include "fermatspiral_strategy.hpp"
#include "fmmm_strategy.hpp"
#include "graph_keys.hpp"
#include "orthogonal_strategy.hpp"
#include "path_analyzer.hpp"
#include "sugiyama_strategy.hpp"
#include "topic_graph_controller.hpp"
#include <QTimer>

TopicGraphController::TopicGraphController(QObject *parent)
    : QObject{parent}, m_graph{new TopicGraph()}, m_layout{new LayoutEngine()},
      m_tgstore{new TGStore()}, m_topicList{new TopicListModel(m_tgstore, this)},
      m_nodeList{new NodeListModel(m_tgstore, this)},
      m_edgeList(new EdgeListModel(this)) {

    if (!m_graph || !m_layout || !m_tgstore || !m_topicList || !m_nodeList || !m_edgeList)
        return;
    m_graph->addObserver(m_tgstore);
    m_graph->addObserver(m_layout);
    // // m_graph.addObserver(m_topicList);
    m_layout->addObserver(m_nodeList);
    m_layout->addObserver(m_edgeList);

    connect(m_tgstore,
            &TGStore::labelUpdated,
            m_nodeList,
            &NodeListModel::onLabelUpdated);
    connect(m_tgstore,
            &TGStore::labelUpdated,
            m_topicList,
            &TopicListModel::onLabelUpdated);
    connect(m_tgstore,
            &TGStore::flagUpdated,
            m_topicList,
            &TopicListModel::onFlagUpdated);

    connect(m_tgstore, &TGStore::flagUpdated, m_nodeList, &NodeListModel::onFlagsUpdated);

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
}
TopicGraphController::~TopicGraphController() { delete m_topicList; }

void TopicGraphController::createTopic(const QString &name) {
    if (m_graph) {
        m_graph->addTopic(name.toStdString());
    }
}
void TopicGraphController::onTopicHoverRequested(uint32_t id, bool isHovered) {
    if (m_tgstore)
        m_tgstore->setTopicState(id, StateFlag::Hovered, isHovered);
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
    if (!m_tgstore)
        return;
    clearSelection();
    m_tgstore->setTopicState(id, StateFlag::Selected, true);
    m_selectedIds.push_back(id);
    m_lastSelectedId = id;
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
    if (m_graph)
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

void TopicGraphController::synchGraphView() {}
void TopicGraphController::directedLayout() {}
void TopicGraphController::treeLayout() {}
void TopicGraphController::circularLayout() {}
void TopicGraphController::planarLayout() {}
void TopicGraphController::defaultLayout() {}
void TopicGraphController::multiLayout() {}
void TopicGraphController::path(const QString &topicA, const QString &topicB) {
    // auto ta = m_graph.getTopic(topicA.toStdString());
    // auto tb = m_graph.getTopic(topicB.toStdString());
    // if (ta == nullptr || tb == nullptr) {
    //     return;
    // }
    // auto parents = TG::PathAnalyzer::dijsktras(m_graph, ta->id, tb->id);
    // auto topicIds = TG::PathAnalyzer::topicPath(parents, tb->id);

    // std::unordered_set<int> topicSet(topicIds.begin(), topicIds.end());
    // for (const auto &topic : m_graph.topics()) {
    //     StateFlag flag = StateFlag::None;
    //     if (topicSet.contains(topic.get()->id))
    //         flag = StateFlag::InPath;
    //     else
    //         flag = StateFlag::Hidden;
    //     m_nodeList->setFlagsOnId(topic.get()->id, flag);
    // }


    // auto edgeKeys = TG::PathAnalyzer::edgePath(topicIds);
    // std::unordered_set<std::string> edgeSet(edgeKeys.begin(), edgeKeys.end());
    // for (const auto &edge : m_graph.edges()) {
    //     StateFlag flag = StateFlag::None;
    //     if (edgeSet.contains(edge.get()->key))
    //         flag = StateFlag::InPath;
    //     else
    //         flag = StateFlag::Hidden;
    //     m_edgeList->setFlagsOnId(edge.get()->key, flag);
    // }
}

void TopicGraphController::calculateHeatScores() {
    // auto map = m_heatScore->computeAllHeatScores();
    // for (const auto [topic, score] : map) {
    //     if (!score)
    //         continue;
    //     m_nodeList->updateHeatScore(topic->id, score);
    // }
}
void TopicGraphController::save(QString fileName) {
    // QString *error = nullptr;
    // m_repo.save(m_graph, fileName, error);
    // if (error) {
    //     qDebug() << error;
    // }
}
void TopicGraphController::load(QString fileName) {
    // clearAll();
    // QString *error = nullptr;

    // TopicGraph temp;
    // bool load = m_repo.load(temp, fileName, error);
    // if (error != nullptr) {
    //     qDebug() << error;
    // }
    // for (auto topic : temp.topics()) {

    //     m_graph.addTopic(topic->id, topic->name, topic->type);

    //     QString name = QString::fromStdString(topic->name);
    //     if (m_topicList) {
    //         m_topicList->addConfirmedItem(topic->id, name);
    //     }

    //     auto gNode = m_layout.addNode(topic->id);
    //     if (m_nodeList) {
    //         m_nodeList->addItem(NodeItem{.label = name,
    //                                      .x = gNode.x,
    //                                      .y = gNode.y,
    //                                      .id = topic->id,
    //                                      .heat = 0});
    //     }
    // }
    // for (const auto &e : temp.edges()) {
    //     auto gData = m_layout.addEdge(e->from, e->to);
    //     if (m_nodeList) {
    //         for (const auto &node : gData.nodes)
    //             m_nodeList->updatePos(node.id, node.x, node.y);
    //     }
    //     if (m_edgeList) {
    //         for (const auto &gedge : gData.edges) {
    //             std::vector<QPointF> points;
    //             for (const auto &ogpoint : gedge.bends) {
    //                 points.push_back({ogpoint.m_x, ogpoint.m_y});
    //             }
    //             m_edgeList->addItem(EdgeItem{
    //                 .key = gedge.key,
    //                 .bends = points,
    //                 .from = gedge.from,
    //                 .to = gedge.to,
    //                 .source_x = gedge.source_x,
    //                 .source_y = gedge.source_y,
    //                 .target_x = gedge.target_x,
    //                 .target_y = gedge.target_y,
    //             });
    //         }
    //     }
    // }
}
void TopicGraphController::clearAll() {
    m_graph->clear();
    m_layout->clear();
    m_topicList->clear();
    synchGraphView();
}
void TopicGraphController::updateBuffer(const QString &buffer) {
    m_currentBuffer = buffer;
}
void TopicGraphController::executeCurrentCommand() {
    if (m_currentBuffer.length() == 0)
        return;
}

QString TopicGraphController::handleAutoComplete() {

    QString buffer = m_currentBuffer.trimmed();
    QStringList parts = buffer.split(" ", Qt::SkipEmptyParts);

    if (parts.isEmpty()) {
        // Autocomplete at start → commands
        return autoCompleteCommand("", parts);
    }

    if (parts.size() == 1) {
        // Completing the first word → command
        return autoCompleteCommand(parts[0], parts);
    }

    // Completing arguments
    QString command = parts[0];
    QString argPrefix = parts.last();

    if (command == "join" || command == "touch" || command == "rm") {
        return autoCompleteTopics(parts, argPrefix);
    }

    return buffer; // Unknown command type
}
QString TopicGraphController::autoCompleteCommand(const QString &buffer,
                                                  const QStringList &parts) {
    static QStringList commands = {"touch", "join", "rm", "ls", "info"};

    QStringList matches;
    for (auto &cmd : commands)
        if (cmd.startsWith(buffer))
            matches << cmd;
    return processAutocomplete(buffer, matches, parts);
}

QString TopicGraphController::autoCompleteTopics(const QStringList &parts,
                                                 const QString &prefix) {
    QStringList matches;

    for (const auto &t : m_graph->topics()) {
        QString name = QString::fromStdString(t->name);
        if (name.startsWith(prefix))
            matches << name;
    }

    return processAutocomplete(prefix, matches, parts);
}
QString TopicGraphController::processAutocomplete(const QString &prefix,
                                                  const QStringList &matches,
                                                  const QStringList &parts) {
    if (matches.isEmpty()) {
        qDebug() << "No matches";
        m_lastPrefix.clear();
        m_lastMatches.clear();
        m_cycleIndex = -1;
        return m_currentBuffer;
    }

    // SAME prefix as last time → cycle
    if (prefix == m_lastPrefix) {
        m_cycleIndex = (m_cycleIndex + 1) % matches.size();
        qDebug() << "Cycle:" << matches[m_cycleIndex];
        return buildNewBuffer(matches[m_cycleIndex], parts);
    }

    // NEW prefix → reset + show suggestions
    m_lastPrefix = prefix;
    m_lastMatches = matches;
    m_cycleIndex = 0;

    if (matches.size() == 1) {
        qDebug() << "Autocomplete:" << matches[0];
    } else {
        qDebug() << "Suggestions:" << matches;
        qDebug() << "Starting with:" << matches[0];
    }

    return buildNewBuffer(matches[0], parts);
}
QString TopicGraphController::buildNewBuffer(const QString &replacement,
                                             const QStringList &parts) {
    QStringList newParts = parts;
    newParts[newParts.size() - 1] = replacement;

    QString newBuffer = newParts.join(" ");
    m_currentBuffer = newBuffer;
    return newBuffer;
}
