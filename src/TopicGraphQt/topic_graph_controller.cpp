#include "fermatspiral_strategy.hpp"
#include "fmmm_strategy.hpp"
#include "orthogonal_strategy.hpp"
#include "path_analyzer.hpp"
#include "sugiyama_strategy.hpp"
#include "topic_graph_controller.hpp"
#include <QTimer>

TopicGraphController::TopicGraphController(QObject *parent)
    : QObject{parent}, m_graph{}, m_layout{}, m_topicStates{}, m_edgeStates{},
      m_topicList{new TopicListModel{&m_topicStates, this}},
      m_nodeList(new NodeListModel(&m_topicStates, this)),
      m_edgeList(new EdgeListModel(&m_edgeStates, this)) {

    if (m_nodeList) {
        connect(&m_topicStates,
                &UIStateManager::stateChanged,
                m_nodeList,
                &NodeListModel::onNodeStateChanged);
    }
    // connect(&m_topicStates,
    //         &UIStateManager::stateChanged,
    //         this,
    //         &TopicGraphController::onStateChanged);

    createTopic("v1");
    createTopic("v2");
    createTopic("v3");
    createTopic("v4");
    createTopic("v5");
    createTopic("v6");
    join("v2", "v1", Edge_Type::ComposedOf);
    join("v1", "v3", Edge_Type::Example);
    join("v3", "v2", Edge_Type::DependsOn);
    join("v3", "v5", Edge_Type::AlternativeTo);
    join("v3", "v4", Edge_Type::DependsOn);
    join("v5", "v2", Edge_Type::ComposedOf);
    join("v5", "v6", Edge_Type::AlternativeTo);
    join("v6", "v4", Edge_Type::Example);
    directedLayout();
}
TopicGraphController::~TopicGraphController() { delete m_topicList; }

void TopicGraphController::createTopic(const QString &name, Topic_Type type) {
    uint32_t id = m_graph.addTopic(name.toStdString(), Topic_Type::Concept);
    if (!id) {
        return;
    }
    if (m_topicList) {
        m_topicList->addConfirmedItem(id, name);
    }

    m_topicStates.setState(std::to_string(id), StateFlag::Selectable);
    m_layout.addNode(id);
    synchGraphView();
}
void TopicGraphController::deleteTopic(const QString &topic) {
    auto ptr = m_graph.getTopic(topic.toStdString());
    if (ptr == nullptr)
        return;
    auto outEdges = m_graph.getOutEdges(ptr->id);
    auto inEdges = m_graph.getInEdges(ptr->id);
    for (auto &edge : outEdges) {
        m_layout.removeEdge(edge.key);
    }
    for (auto &edge : inEdges) {
        m_layout.removeEdge(edge.key);
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
                                Edge_Type type) {
    auto edge = m_graph.addEdge(topicA.toStdString(), topicB.toStdString(), type);
    if (edge == nullptr)
        return;
    m_edgeStates.setState(edge->key, StateFlag::Selectable);
    m_layout.addEdge(edge.get()->from, edge.get()->to);
    synchGraphView();
}
void TopicGraphController::noJoin(const QString &topicA, const QString &topicB) {
    auto ta = m_graph.getTopic(topicA.toStdString());
    auto tb = m_graph.getTopic(topicB.toStdString());
    if (ta == nullptr || tb == nullptr)
        return;
    m_graph.removeEdge(ta->id, tb->id);
    m_layout.removeEdge(std::to_string(ta->id) + "->" + std::to_string(tb->id));
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
//TODO:
//make api on node and edge list for TGC to directly invoke the changes
void TopicGraphController::onStateChanged(const std::string &id,
                                          const StateFlags &flags) {

    const auto &edges = m_graph.getOutEdges(std::stoi(id));
    for (auto edge : edges) {
        m_edgeStates.setState(edge.key, flags);
        m_edgeList->onEdgeStateChanged(edge.from, edge.to);
    }
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
    auto parents = PathAnalyzer::dijsktras(m_graph, ta->id, tb->id);
    auto topics = PathAnalyzer::topicPath(parents, tb->id);

    auto edges = PathAnalyzer::edgePath(topics);

    for (const auto &v : topics) {
        m_topicStates.setState(std::to_string(v), {StateFlag::Highlighted});
    }


    for (auto edge : edges) {
        m_edgeStates.setState(edge, {StateFlag::Highlighted});
        m_edgeList->onEdgeStateChanged(edge);
    }
}