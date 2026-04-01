#include "edge_list_model.hpp"
#include "graph_keys.hpp"

EdgeListModel::EdgeListModel(GraphStore *store, QObject *parent)
    : QAbstractListModel{parent}, m_store(store) {

    connect(m_store, &GraphStore::edgeAdded, this, &EdgeListModel::onEdgeAdded);
    connect(m_store, &GraphStore::edgeDeleted, this, &EdgeListModel::onEdgeRemoved);
    connect(m_store, &GraphStore::clear, this, &EdgeListModel::onClear);
    connect(m_store,
            &GraphStore::edgeTypeUpdated,
            this,
            &EdgeListModel::onEdgeTypeUpdated);
    connect(m_store, &GraphStore::edgeFlagUpdated, this, &EdgeListModel::onFlagUpdated);
    connect(m_store,
            &GraphStore::edgePositionUpdated,
            this,
            &EdgeListModel::onPosUpdated);
}

QHash<int, QByteArray> EdgeListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[EdgePosRole] = "position";
    roles[FlagsRole] = "flags";
    roles[EdgeTypeRole] = "edgeType";
    return roles;
}

Qt::ItemFlags EdgeListModel::flags(const QModelIndex &index) const {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}
int EdgeListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_ids.size();
}

QVariant EdgeListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (index.row() >= m_ids.size() || !m_store) {
        return QVariant();
    }
    auto id = m_ids[index.row()];
    switch (role) {
    case EdgePosRole: return QVariant::fromValue(m_store->edgePos(id));
    case FlagsRole: return static_cast<int>(m_store->flags(id));
    case EdgeTypeRole: return static_cast<int>(m_store->edgeType(id));
    default: return QVariant();
    }
}

void EdgeListModel::onEdgeAdded(const std::string &key) {
    int index = m_ids.size();
    beginInsertRows(QModelIndex(), index, index);
    m_ids.push_back(key);
    endInsertRows();
}
void EdgeListModel::onEdgeRemoved(const std::string &key) {
    auto row = getIndex(key);
    if (row < 0) {
        return;
    }
    const QModelIndex modelIndex = this->index(row);
    beginRemoveRows(modelIndex, row, row);
    m_ids.removeAt(row);
    endRemoveRows();
}
int EdgeListModel::getIndex(const std::string &key) {
    auto it = std::find(m_ids.begin(), m_ids.end(), key);
    if (it == m_ids.end())
        return -1;
    return static_cast<int>(std::distance(m_ids.begin(), it));
}
void EdgeListModel::onFlagUpdated(const std::string &key) {
    int row = getIndex(key);
    if (row < 0)
        return;
    const QModelIndex modelIndex = this->index(row);
    emit dataChanged(modelIndex, modelIndex, {FlagsRole});
}
void EdgeListModel::onEdgeTypeUpdated(const std::string &key) {
    int row = getIndex(key);
    if (row < 0)
        return;
    const QModelIndex modelIndex = this->index(row);
    emit dataChanged(modelIndex, modelIndex, {EdgeTypeRole});
}
void EdgeListModel::onPosUpdated(const std::string &key) {
    int row = getIndex(key);
    if (row < 0)
        return;
    const QModelIndex modelIndex = this->index(row);
    emit dataChanged(modelIndex, modelIndex, {EdgePosRole});
}
void EdgeListModel::onClear() {
    beginResetModel();
    m_ids.clear();
    endResetModel();
}