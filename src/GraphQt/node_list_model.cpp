#include "node_list_model.hpp"

NodeListModel::NodeListModel(GraphStore *store, QObject *parent)
    : QAbstractListModel{parent}, m_store(store) {
    connect(m_store, &GraphStore::nodeAdded, this, &NodeListModel::onNodeAdded);
    connect(m_store, &GraphStore::labelUpdated, this, &NodeListModel::onLabelUpdated);
    connect(m_store, &GraphStore::nodeFlagUpdated, this, &NodeListModel::onFlagsUpdated);
    connect(m_store, &GraphStore::nodePosUpdated, this, &NodeListModel::onPosUpdated);
    connect(m_store, &GraphStore::nodeHeatUpdated, this, &NodeListModel::onHeatUpdated);
    connect(m_store, &GraphStore::nodeDeleted, this, &NodeListModel::onNodeDeleted);
    connect(m_store, &GraphStore::clear, this, &NodeListModel::onClear);
}
QHash<int, QByteArray> NodeListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "nodeId";
    roles[LabelRole] = "label";
    roles[PosRole] = "position";
    roles[FlagsRole] = "flags";
    roles[HeatRole] = "heatScore";
    return roles;
}
int NodeListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_ids.size();
}
Qt::ItemFlags NodeListModel::flags(const QModelIndex &index) const {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QVariant NodeListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (index.row() >= m_ids.size() || !m_store) {
        return QVariant();
    }
    auto id = m_ids[index.row()];

    switch (role) {
    case IdRole: return QVariant::fromValue(id);
    case LabelRole: return m_store->label(id);
    case PosRole: return QVariant::fromValue(m_store->pos(id));
    case FlagsRole: return static_cast<int>(m_store->flags(id));
    case HeatRole: return m_store->heat(id);
    default: return QVariant();
    }
}


void NodeListModel::onNodeAdded(uint32_t id) {
    int row = m_ids.size();
    beginInsertRows(QModelIndex(), row, row);
    m_ids.push_back(id);
    endInsertRows();
}
int NodeListModel::getIndex(uint32_t id) {
    auto it = std::find(m_ids.begin(), m_ids.end(), id);
    if (it == m_ids.end())
        return -1;

    return static_cast<int>(std::distance(m_ids.begin(), it));
}
void NodeListModel::onLabelUpdated(uint32_t id) {
    int row = getIndex(id);
    if (row < 0) {
        return;
    }
    const QModelIndex modelIndex = this->index(row);
    emit dataChanged(modelIndex, modelIndex, {LabelRole});
}
void NodeListModel::onFlagsUpdated(uint32_t id) {
    int row = getIndex(id);
    if (row < 0) {
        return;
    }
    const QModelIndex modelIndex = this->index(row);
    emit dataChanged(modelIndex, modelIndex, {FlagsRole});
}
void NodeListModel::onPosUpdated(uint32_t id) {
    auto row = getIndex(id);
    if (row < 0) {
        return;
    }
    const QModelIndex modelIndex = this->index(row);
    emit dataChanged(modelIndex, modelIndex, {PosRole});
}

void NodeListModel::onHeatUpdated(uint32_t id) {
    auto row = getIndex(id);
    if (row < 0) {
        return;
    }
    const QModelIndex modelIndex = this->index(row);
    emit dataChanged(modelIndex, modelIndex, {HeatRole});
}
void NodeListModel::onNodeDeleted(uint32_t id) {
    auto row = getIndex(id);
    if (row < 0) {
        return;
    }
    const QModelIndex modelIndex = this->index(row);
    beginRemoveRows(modelIndex, row, row);
    m_ids.removeAt(row);
    endRemoveRows();
}
void NodeListModel::onClear() {
    beginResetModel();
    m_ids.clear();
    endResetModel();
}