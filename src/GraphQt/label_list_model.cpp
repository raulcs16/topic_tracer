#include "graph_keys.hpp"
#include "label_list_model.hpp"


LabelListModel::LabelListModel(GraphStore *store, QObject *parent)
    : QAbstractListModel{parent}, m_store(store) {}

QHash<int, QByteArray> LabelListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[LabelRole] = "label";
    roles[FlagsRole] = "flags";
    return roles;
}
int LabelListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return m_ids.size();
}
QVariant LabelListModel::data(const QModelIndex &index, int role) const {
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return QVariant();
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    auto id = m_ids[row];

    switch (role) {
    case IdRole: return id;
    case LabelRole: {
        if (m_store) {
            return m_store->label(id);
        }
        return QVariant();
    }
    case FlagsRole: {
        if (m_store) {
            return static_cast<int>(m_store->flags(id));
        }
        return 0;
    }
    default: return QVariant();
    }
}

Qt::ItemFlags LabelListModel::flags(const QModelIndex &index) const {
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
int LabelListModel::getIndex(uint32_t id) {
    auto it = m_idToRow.find(id);
    if (it == m_idToRow.end()) {
        return -1;
    }
    return it.value();
}

void LabelListModel::onLabelUpdated(uint32_t id) {
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
void LabelListModel::onFlagUpdated(uint32_t id) {
    int index = getIndex(id);
    if (index < 0) {
        return;
    }
    const QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex, modelIndex, {FlagsRole});
}


void LabelListModel::setIsAddingNewTopic(bool value) { m_isAddingNewTopic = value; }


std::vector<uint32_t> LabelListModel::getIdInRange(uint32_t start, uint32_t end) {
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
void LabelListModel::onClear() { clear(); }

void LabelListModel::clear() {
    if (m_ids.isEmpty())
        return;

    beginResetModel();
    m_ids.clear();
    m_idToRow.clear();
    endResetModel();
}
