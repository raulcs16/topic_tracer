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

void TopicGraphController::setTopicListModel(TopicListModel *list) {
    if (m_topicList == list)
        return;

    if (m_topicList)
        disconnect(m_topicList, nullptr, this, nullptr);

    m_topicList = list;
    emit topicListModelChanged();

    if (m_topicList) {
        connect(m_topicList,
                &TopicListModel::requestAddTopic,
                this,
                &TopicGraphController::onRequestAddTopic);
        // connect(m_topicList,
        //         &TopicListModel::requestDeleteTopic,
        //         this,
        //         &TopicGraphController::onRequestDeleteTopic);
    }
}
void TopicGraphController::setGraphController(GraphController *controller) {
    if (m_gr_cntrl == controller)
        return;
    if (m_gr_cntrl)
        disconnect(m_gr_cntrl, nullptr, this, nullptr);
    m_gr_cntrl = controller;
    if (m_gr_cntrl)
        m_gr_cntrl->synchFromTopicGraph(*m_graph);
    emit graphControllerChanged();
}
void TopicGraphController::onRequestAddTopic(int index, const QString &name) {
    uint32_t id = m_graph->addTopic(name.toStdString());
    if (!id)
        return;

    if (m_topicList)
        m_topicList->confirmTopic(index, id);
    if (m_gr_cntrl)
        m_gr_cntrl->synchFromTopicGraph(*m_graph);
}