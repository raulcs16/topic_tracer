#include "topic_graph_qt.hpp"

int TopicGraphQt::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    // The number of rows is the number of topics in the graph
    return m_graph->numTopics();
}


QVariant TopicGraphQt::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    // Get the topic from your TopicGraph using the row index
    // This is a simplified example; your TopicGraph will need a way to get a topic by its index.
    // A better approach would be to store the IDs in a list and use those.
    uint32_t topicId = m_graph->getAllTopicIds().at(index.row());
    std::shared_ptr<Topic> topic = m_graph->getTopic(topicId);

    if (role == Qt::DisplayRole) {
        return QString::fromStdString(topic->getName());
    }
    // You could handle other roles here, like Qt::CheckStateRole for the 'covered' property
    return QVariant();
}


void TopicGraphQt::addTopic(const QString &name, Topic_Type topic_type) {
    // First, notify the view that we are about to insert a new row
    int newRow = m_graph->numTopics();
    beginInsertRows(QModelIndex(), newRow, newRow);

    // Call the underlying agnostic function
    std::string name_std = name.toStdString();
    m_graph->addTopic(name_std, topic_type);

    // Get the newly added topic and create a Qt wrapper for it
    // Note: your TopicGraph needs a way to return the newly added topic or its ID
    uint32_t newTopicId = m_graph->getTopic(name_std)->getId(); // Example, may need to be adjusted
    std::shared_ptr<Topic> newTopic = m_graph->getTopic(newTopicId);
    QSharedPointer<TopicQt> newTopicQt = QSharedPointer<TopicQt>::create(newTopic);
    m_topics.append(newTopicQt);

    // Finish the insertion, which triggers the view to update
    endInsertRows();

    // Emit a custom signal if other parts of the application need to react
    emit topicAdded(newTopicId);
}

void TopicGraphQt::removeTopic(uint32_t id) {}
void TopicGraphQt::addEdge(uint32_t from, uint32_t to, Edge_Type type) {}