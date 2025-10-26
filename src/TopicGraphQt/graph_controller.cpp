#include "graph_controller.hpp"

GraphController::GraphController(QObject *parent)
    : QObject{parent}, m_nodeModel{this}, m_edgeModel{this}, m_engine{} {}


void GraphController::synchFromTopicGraph(TopicGraph &graph) {
    auto topics = graph.topics();
    auto tedges = graph.edges();

    std::vector<std::reference_wrapper<GraphNode>> gnodes;
    std::vector<std::reference_wrapper<GraphEdge>> gedges;
    for (size_t i = 0; i < topics.size(); i++) {
        gnodes.push_back(topics[i].get().geometry);
    }
    for (size_t i = 0; i < tedges.size(); i++) {
        gedges.push_back(tedges[i].get().geometry);
    }

    m_engine.calculateLayout(gnodes, gedges);
    m_nodeModel.resetNodes(graph.graphNodes());
    m_edgeModel.resetEdges(graph.graphEdges());
    emit graphChanged();
}