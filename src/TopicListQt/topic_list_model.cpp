#include "topic.hpp"
#include "topic_list_model.hpp"


TopicListModel::TopicListModel(ITopicGraphReader &graph, QObject *parent)
    : QAbstractListModel{parent}, m_graph{graph} {}

int TopicListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return m_graph.numTopics();
}
QVariant TopicListModel::data(const QModelIndex &index, int role) const {
    if (!checkIndex(index, CheckIndexOption::IndexIsValid)) {
        return QVariant();
    }
    if (!index.isValid()) {
        return QVariant();
    }

    int row = index.row();

    auto topic = m_graph[row];

    if (!topic) {
        qDebug() << "not a topic";
        return QVariant();
    }

    switch (role) {
    case NameRole: return QString::fromStdString(topic->name());
    case IdRole: return topic->id();
    case CoveredRole: return topic->covered();
    case TypeRole:
        // You'll need to convert your enum to a QVariant type (e.g., int or QString).
        return static_cast<int>(topic->type());
    default: return QVariant();
    }
}

Qt::ItemFlags TopicListModel::flags(const QModelIndex &index) const {}
bool TopicListModel::setData(const QModelIndex &index, const QVariant &value, int role) {}

QHash<int, QByteArray> TopicListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "topicId";
    roles[NameRole] = "topicName";
    roles[CoveredRole] = "isCovered";
    roles[TypeRole] = "topicType";
    return roles;
}