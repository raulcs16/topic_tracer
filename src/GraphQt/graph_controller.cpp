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
    GraphState state;
    for (uint32_t i = 0; i < 100; i++) {
        state.nodes.push_back(GraphNode{.id = i});
    }
    state.edges.push_back(GraphEdge{.from = 0, .to = 3});
    state.edges.push_back(GraphEdge{.from = 0, .to = 5});
    state.edges.push_back(GraphEdge{.from = 0, .to = 7});
    m_engine.setState(state);
    m_engine.calculateLayout();
    invalidateLayout();
}
