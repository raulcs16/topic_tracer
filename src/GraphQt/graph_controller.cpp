#include "graph_controller.hpp"

GraphController::GraphController(QObject *parent)
    : QObject{parent}, m_nodeModel{this}, m_edgeModel{this}, m_engine{} {

    init();
}

void GraphController::invalidateLayout() {
    m_nodeModel.resetNodes(m_engine.nodes());
    m_edgeModel.resetEdges(m_engine.edges());
    emit graphChanged();
}
void GraphController::init() {
    //example
    m_state.nodes.push_back(GraphNode{.id = 100, .label = "DesignPatterns"});
    m_state.nodes.push_back(GraphNode{.id = 200, .label = "StructualPatterns"});
    m_state.nodes.push_back(GraphNode{.id = 300, .label = "CreationalPatterns"});
    m_state.nodes.push_back(GraphNode{.id = 400, .label = "BehaviouralPatterns"});

    m_state.edges.push_back(GraphEdge{.from = 100, .to = 200});
    m_state.edges.push_back(GraphEdge{.from = 100, .to = 300});
    m_state.edges.push_back(GraphEdge{.from = 100, .to = 400});

    m_state.nodes.push_back(GraphNode{.id = 5, .label = "Adapter"});
    m_state.nodes.push_back(GraphNode{.id = 6, .label = "Bridge"});
    m_state.nodes.push_back(GraphNode{.id = 7, .label = "Composite"});
    m_state.nodes.push_back(GraphNode{.id = 8, .label = "Decorator"});
    m_state.nodes.push_back(GraphNode{.id = 9, .label = "Facade"});
    m_state.nodes.push_back(GraphNode{.id = 10, .label = "Flyweight"});
    m_state.nodes.push_back(GraphNode{.id = 11, .label = "Proxy"});

    m_state.edges.push_back(GraphEdge{.from = 200, .to = 5});
    m_state.edges.push_back(GraphEdge{.from = 200, .to = 6});
    m_state.edges.push_back(GraphEdge{.from = 200, .to = 7});
    m_state.edges.push_back(GraphEdge{.from = 200, .to = 8});
    m_state.edges.push_back(GraphEdge{.from = 200, .to = 9});
    m_state.edges.push_back(GraphEdge{.from = 200, .to = 10});
    m_state.edges.push_back(GraphEdge{.from = 200, .to = 11});

    m_state.nodes.push_back(GraphNode{.id = 12, .label = "AbstractFactory"});
    m_state.nodes.push_back(GraphNode{.id = 13, .label = "Builder"});
    m_state.nodes.push_back(GraphNode{.id = 14, .label = "FactoryMethod"});
    m_state.nodes.push_back(GraphNode{.id = 15, .label = "Prototype"});
    m_state.nodes.push_back(GraphNode{.id = 16, .label = "Singleton"});

    m_state.edges.push_back(GraphEdge{.from = 300, .to = 12});
    m_state.edges.push_back(GraphEdge{.from = 300, .to = 13});
    m_state.edges.push_back(GraphEdge{.from = 300, .to = 14});
    m_state.edges.push_back(GraphEdge{.from = 300, .to = 15});
    m_state.edges.push_back(GraphEdge{.from = 300, .to = 16});

    m_state.nodes.push_back(GraphNode{.id = 17, .label = "ChainOfResp."});
    m_state.nodes.push_back(GraphNode{.id = 18, .label = "Command"});
    m_state.nodes.push_back(GraphNode{.id = 19, .label = "Iterator"});
    m_state.nodes.push_back(GraphNode{.id = 20, .label = "Mediator"});
    m_state.nodes.push_back(GraphNode{.id = 21, .label = "Memento"});
    m_state.nodes.push_back(GraphNode{.id = 22, .label = "Observer"});
    m_state.nodes.push_back(GraphNode{.id = 23, .label = "Strategy"});
    m_state.nodes.push_back(GraphNode{.id = 24, .label = "State"});
    m_state.nodes.push_back(GraphNode{.id = 25, .label = "TemplateMethod"});
    m_state.nodes.push_back(GraphNode{.id = 26, .label = "Visitor"});
    m_state.nodes.push_back(GraphNode{.id = 26, .label = "Interpreter"});

    m_state.edges.push_back(GraphEdge{.from = 400, .to = 17});
    m_state.edges.push_back(GraphEdge{.from = 400, .to = 18});
    m_state.edges.push_back(GraphEdge{.from = 400, .to = 19});
    m_state.edges.push_back(GraphEdge{.from = 400, .to = 20});
    m_state.edges.push_back(GraphEdge{.from = 400, .to = 21});
    m_state.edges.push_back(GraphEdge{.from = 400, .to = 22});
    m_state.edges.push_back(GraphEdge{.from = 400, .to = 23});
    m_state.edges.push_back(GraphEdge{.from = 400, .to = 24});
    m_state.edges.push_back(GraphEdge{.from = 400, .to = 25});
    m_state.edges.push_back(GraphEdge{.from = 400, .to = 26});


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