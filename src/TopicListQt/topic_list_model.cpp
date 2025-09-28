#include "topic_list_model.hpp"


TopicListModel::TopicListModel(QObject *parent) : QAbstractListModel{parent} {
    m_topics.append(TopicItem{.id = m_next_id++, .name = "SomeTopic"});
    m_topics.append(TopicItem{.id = m_next_id++, .name = "SomeTopic"});
    m_topics.append(TopicItem{.id = m_next_id++, .name = "SomeTopic"});
    m_topics.append(TopicItem{.id = m_next_id++, .name = "SomeTopic"});
}

int TopicListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return m_topics.size();
}
QVariant TopicListModel::data(const QModelIndex &index, int role) const {
    if (!checkIndex(index, CheckIndexOption::IndexIsValid)) {
        return QVariant();
    }
    if (!index.isValid()) {
        return QVariant();
    }

    int row = index.row();

    auto topic = m_topics[row];

    switch (role) {
    case NameRole: return topic.name;
    case IdRole: return topic.id;
    default: return QVariant();
    }
}

Qt::ItemFlags TopicListModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    // Items can be selected and enabled.
    Qt::ItemFlags baseFlags =
        QAbstractListModel::flags(index) | Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    // Allow editing of the NameRole
    if (index.isValid()) {
        baseFlags |= Qt::ItemIsEditable;
    }
    return baseFlags;
}
bool TopicListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || index.row() >= m_topics.size()) {
        return false;
    }

    TopicItem &item = m_topics[index.row()];
    bool changed = false;

    if (role == NameRole) {
        QString newName = value.toString().trimmed();
        if (item.name != newName) {
            item.name = newName;
            changed = true;
        }
    }

    if (changed) {
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

QHash<int, QByteArray> TopicListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "topicId";
    roles[NameRole] = "topicName";
    return roles;
}
void TopicListModel::addItem(const QString &name) {
    QString topicName = name.trimmed();
    if (topicName.isEmpty()) {
        qWarning() << "Cannot add a topic with an empty name.";
        return;
    }

    const int newRow = m_topics.size();

    beginInsertRows(QModelIndex(), newRow, newRow);

    m_topics.push_back(TopicItem{
        .id = m_next_id++,
        .name = topicName,
    });

    endInsertRows();
}

bool TopicListModel::removeItem(int index) {
    if (index < 0 || index >= m_topics.size()) {
        return false;
    }

    beginRemoveRows(QModelIndex(), index, index);

    m_topics.removeAt(index);

    endRemoveRows();

    return true;
}

bool TopicListModel::editItem(int index, const QString &newName) {
    if (index < 0 || index >= m_topics.size() || newName.trimmed().isEmpty()) {
        return false;
    }
    QModelIndex modelIndex = this->index(index);
    return setData(modelIndex, newName.trimmed(), NameRole);
}