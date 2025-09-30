#include "topic_list_model.hpp"


TopicListModel::TopicListModel(QObject *parent) : QAbstractListModel{parent} {}

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
    case PendingRole: return topic.pending;
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
    roles[PendingRole] = "pending";
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

    int temp = m_next_temp--;
    m_topics.push_back(
        TopicItem{.tempId = temp, .id = 0, .name = topicName, .pending = true});

    endInsertRows();
    m_controller->addTopic(temp, topicName);
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

void TopicListModel::setEditingIndex(int idx) {
    if (idx == m_editingIndex)
        return;
    //-1 non are set
    if (idx < -1 || idx >= m_topics.size())
        return;

    m_editingIndex = idx;
    emit editingIndexChanged();
}

void TopicListModel::setIsAddingNewTopic(bool value) {
    if (value != m_isAddingNewTopic) {
        m_isAddingNewTopic = value;
        emit isAddingNewTopicChanged();
    }
}
void TopicListModel::setCurrentIndex(int idx) {
    if (idx == m_currentIndex)
        return;
    //-1 no currentIndex
    if (idx < -1 || idx >= m_topics.size())
        return;
    m_currentIndex = idx;
    emit currentIndexChanged();
}
TopicGraphController *TopicListModel::controller() const { return m_controller; }
void TopicListModel::setController(TopicGraphController *ctrl) {
    if (m_controller == ctrl)
        return;
    if (m_controller)
        disconnect(m_controller, nullptr, this, nullptr);

    m_controller = ctrl;

    if (m_controller) {
        // connect signals here, e.g.
        connect(m_controller,
                &TopicGraphController::topicAdded,
                this,
                &TopicListModel::onTopicAdded);
    }

    emit controllerChanged();
}
void TopicListModel::onTopicAdded(int tempId, bool success, uint32_t id) {
    for (int i = 0; i < m_topics.size(); ++i) {
        if (m_topics[i].tempId == tempId) {
            if (success) {
                m_topics[i].id = id;
                m_topics[i].pending = false;

                QModelIndex idx = index(i);
                emit dataChanged(idx, idx, {IdRole, PendingRole});
            } else {
                beginRemoveRows(QModelIndex(), i, i);
                m_topics.removeAt(i);
                endRemoveRows();
            }
            break;
        }
    }
}