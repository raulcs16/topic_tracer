#include "fermatspiral_strategy.hpp"
#include "fmmm_strategy.hpp"
#include "graph_keys.hpp"
#include "orthogonal_strategy.hpp"
#include "path_analyzer.hpp"
#include "sugiyama_strategy.hpp"
#include "topic_graph_controller.hpp"
#include <QTimer>

TopicGraphController::TopicGraphController(QObject *parent)
    : QObject{parent}, m_graph{}, m_layout{}, m_topicList{new TopicListModel{this}},
      m_nodeList(new NodeListModel(this)), m_edgeList(new EdgeListModel(this)),
      m_heatScore(new HeatScoreSystem{m_evidenceDb, m_graph}), m_repo("data") {


    connect(m_topicList,
            &TopicListModel::topicHovered,
            this,
            &TopicGraphController::onTopicHovered);

    connect(m_topicList,
            &TopicListModel::topicUnHovered,
            this,
            &TopicGraphController::onTopicUnHovered);

    connect(m_topicList,
            &TopicListModel::topicSelected,
            this,
            &TopicGraphController::onTopicSelected);

    connect(m_topicList,
            &TopicListModel::topicUnSelected,
            this,
            &TopicGraphController::onTopicUnSelected);
    connect(m_topicList,
            &TopicListModel::topicDeleted,
            this,
            &TopicGraphController::onTopicDeleted);
    connect(m_topicList,
            &TopicListModel::topicRenamed,
            this,
            &TopicGraphController::onTopicRenamed);
}
TopicGraphController::~TopicGraphController() { delete m_topicList; }

void TopicGraphController::createTopic(const QString &name, TopicType type) {
    auto topic = m_graph.addTopic(name.toStdString(), type);
    if (!topic) {
        return;
    }
    if (m_topicList) {
        m_topicList->addConfirmedItem(topic->id, name);
    }

    m_layout.addNode(topic->id);
    synchGraphView();
}
void TopicGraphController::deleteTopic(const QString &topic) {
    auto ptr = m_graph.getTopic(topic.toStdString());
    if (ptr == nullptr)
        return;
    auto outEdges = m_graph.getOutEdges(ptr->id);
    auto inEdges = m_graph.getInEdges(ptr->id);
    for (auto &edge : outEdges) {
        m_layout.removeEdge(edge->key);
    }
    for (auto &edge : inEdges) {
        m_layout.removeEdge(edge->key);
    }
    m_layout.removeNode(ptr->id);
    m_graph.deleteTopic(ptr->id);
    if (m_topicList) {
        m_topicList->deleteTopic(ptr->id);
    }
    synchGraphView();
}
void TopicGraphController::rename(const QString &topic, const QString &new_name) {
    auto ptr = m_graph.getTopic(topic.toStdString());
    if (ptr == nullptr) {

        return;
    }
    bool success = m_graph.renameTopic(ptr->id, new_name.toStdString());
    if (!success) {
        return;
    }
    if (m_topicList) {
        m_topicList->renameTopic(ptr->id, new_name);
    }
    synchGraphView();
}

void TopicGraphController::join(const QString &topicA,
                                const QString &topicB,
                                EdgeType type) {
    auto edge = m_graph.addEdge(topicA.toStdString(), topicB.toStdString(), type);
    if (edge == nullptr)
        return;
    m_layout.addEdge(edge.get()->from, edge.get()->to);
    synchGraphView();
}
void TopicGraphController::noJoin(const QString &topicA, const QString &topicB) {
    auto ta = m_graph.getTopic(topicA.toStdString());
    auto tb = m_graph.getTopic(topicB.toStdString());
    if (ta == nullptr || tb == nullptr)
        return;
    m_graph.removeEdge(ta->id, tb->id);
    m_layout.removeEdge(GraphKeys::key(ta->id, tb->id));
    synchGraphView();
}

void TopicGraphController::synchGraphView() {
    if (!m_nodeList || !m_edgeList)
        return;
    m_layout.calculateLayout();
    std::vector<EdgeItem> edgeList;
    auto gEdges = m_layout.edges();
    for (const auto &gedge : gEdges) {
        std::vector<QPointF> points;
        for (const auto &ogpoint : gedge.bends) {
            points.push_back({ogpoint.m_x, ogpoint.m_y});
        }
        edgeList.push_back(EdgeItem{
            .key = gedge.key,
            .bends = points,
            .from = gedge.from,
            .to = gedge.to,
            .source_x = gedge.source_x,
            .source_y = gedge.source_y,
            .target_x = gedge.target_x,
            .target_y = gedge.target_y,
        });
    }
    m_edgeList->resetEdges(edgeList);

    auto gNodes = m_layout.nodes();
    std::vector<NodeItem> nodeList;
    for (const auto &gnode : gNodes) {
        auto ptr = m_graph.getTopic(gnode.id);
        if (ptr == nullptr)
            continue;
        QString label = QString::fromStdString(ptr->name);
        nodeList.push_back(
            NodeItem{.id = gnode.id, .x = gnode.x, .y = gnode.y, .label = label});
    }
    m_nodeList->resetNodes(nodeList);
}


void TopicGraphController::directedLayout() {
    m_layout.setStrategy(std::make_unique<FMMMStrategy>(m_layout.ogdfContext()));
    synchGraphView();
}
void TopicGraphController::treeLayout() {}
void TopicGraphController::circularLayout() {}
void TopicGraphController::planarLayout() {
    m_layout.setStrategy(std::make_unique<OrthogonalStrategy>(m_layout.ogdfContext()));
    synchGraphView();
}
void TopicGraphController::defaultLayout() {
    m_layout.setStrategy(std::make_unique<FermatSpiralStrategy>());
    synchGraphView();
}
void TopicGraphController::multiLayout() {
    m_layout.setStrategy(std::make_unique<SugiyamaStrategy>(m_layout.ogdfContext()));
    synchGraphView();
}
void TopicGraphController::path(const QString &topicA, const QString &topicB) {
    auto ta = m_graph.getTopic(topicA.toStdString());
    auto tb = m_graph.getTopic(topicB.toStdString());
    if (ta == nullptr || tb == nullptr) {
        return;
    }
    auto parents = TG::PathAnalyzer::dijsktras(m_graph, ta->id, tb->id);
    auto topicIds = TG::PathAnalyzer::topicPath(parents, tb->id);

    std::unordered_set<int> topicSet(topicIds.begin(), topicIds.end());
    for (const auto &topic : m_graph.topics()) {
        StateFlag flag = StateFlag::None;
        if (topicSet.contains(topic.get()->id))
            flag = StateFlag::InPath;
        else
            flag = StateFlag::Hidden;
        m_nodeList->setFlagsOnId(topic.get()->id, flag);
    }


    auto edgeKeys = TG::PathAnalyzer::edgePath(topicIds);
    std::unordered_set<std::string> edgeSet(edgeKeys.begin(), edgeKeys.end());
    for (const auto &edge : m_graph.edges()) {
        StateFlag flag = StateFlag::None;
        if (edgeSet.contains(edge.get()->key))
            flag = StateFlag::InPath;
        else
            flag = StateFlag::Hidden;
        m_edgeList->setFlagsOnId(edge.get()->key, flag);
    }
}

void TopicGraphController::onTopicHovered(uint32_t id) {
    m_nodeList->setFlagsOnId(id, StateFlag::Hovered);
    auto outEdges = m_graph.getOutEdges(id);
    for (const auto &e : outEdges) {
        m_edgeList->setFlagsOnId(e->key, StateFlag::Hovered);
    }
}

void TopicGraphController::onTopicUnHovered(uint32_t id) {
    m_nodeList->unSetFlagsOnId(id, StateFlag::Hovered);
    auto outEdges = m_graph.getOutEdges(id);
    for (const auto &e : outEdges) {
        m_edgeList->unSetFlagsOnId(e->key, StateFlag::Hovered);
    }
}
void TopicGraphController::onTopicSelected(uint32_t id) {
    m_nodeList->setFlagsOnId(id, StateFlag::Selected);
}
void TopicGraphController::onTopicUnSelected(uint32_t id) {
    m_nodeList->unSetFlagsOnId(id, StateFlag::Selected);
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
    QString *error = nullptr;
    m_repo.save(m_graph, fileName, error);
    if (error) {
        qDebug() << error;
    }
}
void TopicGraphController::load(QString fileName) {
    clearAll();
    QString *error = nullptr;

    bool load = m_repo.load(m_graph, fileName, error);
    if (error != nullptr) {
        qDebug() << error;
    }
    for (auto topic : m_graph.topics()) {
        if (m_topicList) {
            m_topicList->addConfirmedItem(topic->id, QString::fromStdString(topic->name));
        }

        m_layout.addNode(topic->id);
    }
    for (auto edge : m_graph.edges()) {
        m_layout.addEdge(edge.get()->from, edge.get()->to);
    }
    synchGraphView();
}
void TopicGraphController::clearAll() {
    m_graph.clear();
    m_layout.clear();
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

    for (const auto &t : m_graph.topics()) {
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
void TopicGraphController::onTopicDeleted(uint32_t id) {
    auto in = m_graph.getOutEdges(id);
    auto out = m_graph.getInEdges(id);
    m_graph.deleteTopic(id);
    if (m_nodeList) {
        m_nodeList->deleteNode(id);
    }
    if (m_edgeList) {
        for (const auto &e : in) {
            m_edgeList->deleteEdge(e->key);
        }
        for (const auto &e : out) {
            m_edgeList->deleteEdge(e->key);
        }
    }
}
void TopicGraphController::onTopicRenamed(uint32_t id, const QString &label) {
    bool success = m_graph.renameTopic(id, label.toStdString());
    if (!success)
        return;
    if (m_nodeList) {
        m_nodeList->updateLabel(id, label);
    }
}
