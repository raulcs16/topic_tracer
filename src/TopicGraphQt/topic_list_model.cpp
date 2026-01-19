#include "graph_keys.hpp"
#include "topic_list_model.hpp"


TopicListModel::TopicListModel(TGStore *store, QObject *parent)
    : QAbstractListModel{parent}, m_tgstore(store) {}

QHash<int, QByteArray> TopicListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "topicId";
    roles[LabelRole] = "topicName";
    roles[FlagsRole] = "flags";
    return roles;
}
int TopicListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return m_ids.size();
}
QVariant TopicListModel::data(const QModelIndex &index, int role) const {
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return QVariant();
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    auto id = m_ids[row];

    switch (role) {
    case IdRole: return id;
    case LabelRole: {
        if (m_tgstore) {
            return m_tgstore->label(id);
        }
        return QVariant();
    }
    case FlagsRole: {
        if (m_tgstore) {
            return static_cast<int>(m_tgstore->flags(id));
        }
        return 0;
    }
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
int TopicListModel::getIndex(uint32_t id) {
    auto it = m_idToRow.find(id);
    if (it == m_idToRow.end()) {
        return -1;
    }
    return it.value();
}

void TopicListModel::onLabelUpdated(uint32_t id) {
    int index = getIndex(id);
    if (index >= 0) {
        const QModelIndex modelIndex = this->index(index);
        emit dataChanged(modelIndex, modelIndex, {LabelRole});
        return;
    }
    const int newRow = m_ids.size();
    beginInsertRows(QModelIndex(), newRow, newRow);
    m_ids.push_back(id);
    endInsertRows();
    m_idToRow.insert(id, newRow);
}
void TopicListModel::onFlagUpdated(uint32_t id) {
    int index = getIndex(id);
    if (index < 0) {
        return;
    }
    const QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex, modelIndex, {FlagsRole});
}

void TopicListModel::selectIndex(int index) {
    if (index < 0 || index >= m_ids.size() || index == m_lastSelectedIndex)
        return;
    clearSelection();
    m_tgstore->setSelected(m_ids[index]);
    m_selectedIndexes.push_back(index);
    m_lastSelectedIndex = index;
}
void TopicListModel::setIsAddingNewTopic(bool value) { m_isAddingNewTopic = value; }
void TopicListModel::setHovered(uint32_t id) {
    m_tgstore->setTopicState(id, StateFlag::Hovered, true);
}

void TopicListModel::unsetHovered(uint32_t id) {
    m_tgstore->setTopicState(id, StateFlag::Hovered, false);
}
void TopicListModel::toggleSelect(int index) {
    // if (index < 0 || index >= m_topics.size())
    //     return;
    // if (m_stateFlags[m_topics[index].id].has(StateFlag::Selected)) {
    //     removeFlags(index, StateFlag::Selected);
    //     emit topicUnSelected(m_topics[index].id);
    //     auto it = std::find(m_selectedIndexes.begin(), m_selectedIndexes.end(), index);
    //     if (it != m_selectedIndexes.end()) {
    //         m_selectedIndexes.erase(it);
    //     }
    //     return;
    // }
    // addFlags(index, StateFlag::Selected);
    // emit topicSelected(m_topics[index].id);
    // m_selectedIndexes.push_back(index);
    // m_lastSelectedIndex = index;
}
void TopicListModel::rangeSelect(int target) {
    // if (target < 0 || target >= m_topics.size() || target == m_lastSelectedIndex) {
    //     return;
    // }

    // //remove previous range
    // if (m_rangeSelectedIndex != -1) {
    //     int min = qMin(m_lastSelectedIndex, m_rangeSelectedIndex);
    //     int max = qMax(m_lastSelectedIndex, m_rangeSelectedIndex);
    //     std::vector<int> toRemove;
    //     toRemove.reserve(max - min + 1);
    //     for (int i : m_selectedIndexes) {
    //         if (i >= min && i <= max && i != m_lastSelectedIndex) {
    //             toRemove.push_back(i);
    //         }
    //     }
    //     for (int i : toRemove) {
    //         removeFlags(i, StateFlag::Selected);
    //         emit topicUnSelected(m_topics[i].id);
    //         auto it = std::find(m_selectedIndexes.begin(), m_selectedIndexes.end(), i);
    //         if (it != m_selectedIndexes.end()) {
    //             m_selectedIndexes.erase(it);
    //         }
    //     }
    // }

    // //add range
    // int min = qMin(m_lastSelectedIndex, target);
    // int max = qMax(m_lastSelectedIndex, target);
    // for (; min <= max; min++) {
    //     if (min == m_lastSelectedIndex)
    //         continue;
    //     emit topicSelected(m_topics[min].id);
    //     addFlags(min, StateFlag::Selected);
    //     m_selectedIndexes.push_back(min);
    // }
    // m_rangeSelectedIndex = target;
}
void TopicListModel::clearSelection() {
    // for (auto i : m_selectedIndexes) {
    //     removeFlags(i, StateFlag::Selected);
    //     emit topicUnSelected(m_topics[i].id);
    // }
    // m_selectedIndexes.clear();
    // m_lastSelectedIndex = -1;
    // m_rangeSelectedIndex = -1;
}


void TopicListModel::clear() {
    if (m_ids.isEmpty())
        return;

    beginResetModel();
    m_ids.clear();
    m_idToRow.clear();
    endResetModel();
}
