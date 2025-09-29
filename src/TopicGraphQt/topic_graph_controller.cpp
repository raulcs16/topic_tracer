#include "topic_graph_controller.hpp"

TopicGraphController::TopicGraphController(TopicGraph *graph, QObject *parent)
    : m_graph{graph}, QObject{parent} {}

void TopicGraphController::addTopic(const QString &name, Topic_Type type) {

    auto id = m_graph->addTopic(name.toStdString(), type);

    if (id > 0)
        emit topicAdded(true, id);
}

void TopicGraphController::renameTopic(uint32_t id, const QString &new_name) {

    bool success = m_graph->renameTopic(id, new_name.toStdString());

    if (success) {
        emit topicRenamed(true, id);
    }
}
void TopicGraphController::deleteTopic(uint32_t id) { emit topicDelelted(true, id); }