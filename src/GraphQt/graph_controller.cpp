#include "graph_controller.hpp"

GraphController::GraphController(QObject *parent)
    : QObject{parent}, m_nodeModel{this}, m_edgeModel{this}, m_engine{} {

    connect(this,
            &GraphController::graphChanged,
            &m_nodeModel,
            &NodeListModel::onGaphChanged);
    init();
}

void GraphController::invalidateLayout() {
    m_nodeModel.resetNodes(m_engine.nodes());
    m_edgeModel.resetEdges(m_engine.edges());
    // emit graphChanged();
}
void GraphController::init() {
    //
    for (uint32_t i = 0; i < 10; i++) {
        m_state.nodes.push_back(GraphNode{.id = i, .label = "Topic" + std::to_string(i)});
    }
    m_state.edges.push_back(GraphEdge{.from = 0, .to = 3});
    m_state.edges.push_back(GraphEdge{.from = 0, .to = 5});
    m_state.edges.push_back(GraphEdge{.from = 0, .to = 7});
    m_engine.setState(m_state);
    m_engine.calculateLayout();
    invalidateLayout();
}
void GraphController::addNode(uint32_t id, const QString &label) {
    m_state.nodes.push_back(GraphNode{.id = id, .label = label.toStdString()});
    m_engine.setState(m_state);
    m_engine.calculateLayout();
    invalidateLayout();
}