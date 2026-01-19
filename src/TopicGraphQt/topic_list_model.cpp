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


void TopicListModel::setIsAddingNewTopic(bool value) { m_isAddingNewTopic = value; }


std::vector<uint32_t> TopicListModel::getIdInRange(uint32_t start, uint32_t end) {
    auto startIt = std::find(m_ids.begin(), m_ids.end(), start);
    auto endIt = std::find(m_ids.begin(), m_ids.end(), end);
    if (startIt == m_ids.end() || endIt == m_ids.end()) {
        return {};
    }
    auto min = std::min(startIt, endIt);
    auto max = std::max(startIt, endIt);
    std::vector<uint32_t> ids;
    for (; min <= max; min++) {
        ids.push_back(*min);
    }
    return ids;
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
