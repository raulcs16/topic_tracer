#include "topic_graph_controller.hpp"
#include <QTimer>

TopicGraphController::TopicGraphController(QObject *parent) : QObject{parent} {
    m_graph = new TopicGraph();
}
TopicGraphController::~TopicGraphController() { delete m_graph; }
void TopicGraphController::addTopic(int tempId, const QString &name, Topic_Type type) {

    auto id = m_graph->addTopic(name.toStdString(), type);


    emit topicAdded(tempId, id > 0, id);
}

void TopicGraphController::renameTopic(uint32_t id, const QString &new_name) {

    bool success = m_graph->renameTopic(id, new_name.toStdString());

    if (success) {
        emit topicRenamed(true, id);
    }
}
void TopicGraphController::deleteTopic(uint32_t id) { emit topicDelelted(true, id); }