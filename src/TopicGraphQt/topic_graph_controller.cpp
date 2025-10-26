#include "topic_graph_controller.hpp"
#include <QTimer>

TopicGraphController::TopicGraphController(QObject *parent)
    : QObject{parent}, m_graph{}, m_layout{}, m_topicList{new TopicListModel(this)},
      m_nodeList(new NodeListModel(this)), m_edgeList(new EdgeListModel(this)) {}
TopicGraphController::~TopicGraphController() { delete m_topicList; }

void TopicGraphController::createTopic(const QString &name, Topic_Type type) {
    uint32_t id = m_graph.addTopic(name.toStdString(), Topic_Type::Concept);
    if (!id) {
        return;
    }
    if (m_topicList) {
        m_topicList->addConfirmedItem(id, name);
    }
    m_layout.addNode(id);
    synchGraphView();
}

void TopicGraphController::join(const QString &topicA, const QString &topicB) {
    auto edge =
        m_graph.addEdge(topicA.toStdString(), topicB.toStdString(), Edge_Type::RelatedTo);
    if (edge == nullptr)
        return;
    m_layout.addEdge(edge.get()->from, edge.get()->to);
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