#include "topic_graph_controller.hpp"

TopicGraphController::TopicGraphController(TopicGraph *graph, QObject *parent)
    : m_graph{graph}, QObject{parent} {}

void TopicGraphController::addTopic(const QString &name, Topic_Type type) {

    auto id = m_graph->addTopic(name.toStdString(), type);

    if (id > 0)
        emit topicAdded(true, id);
}