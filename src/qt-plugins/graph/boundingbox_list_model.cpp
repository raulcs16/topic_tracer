#include "boundingbox_list_model.hpp"

BoundingBoxListModel::BoundingBoxListModel(NodeTypePresentor *presentor)
    : QAbstractListModel{presentor}, m_presentor(presentor) {
    connect(m_presentor,
            &NodeTypePresentor::onNewTypeCreated,
            this,
            &BoundingBoxListModel::onBoxAdded);
    // connect(m_store, &GraphStore::clear, this, &BoundingBoxListModel::onClear);
    // connect(m_store, &GraphStore::boxAdded, this, &BoundingBoxListModel::onBoxAdded);
    // connect(m_store, &GraphStore::boxDeleted, this, &BoundingBoxListModel::onBoxRemoved);
    // connect(m_store, &GraphStore::boxUpdated, this, &BoundingBoxListModel::onBoxUpdated);
}

QHash<int, QByteArray> BoundingBoxListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[LabelRole] = "label";
    roles[RectRole] = "rect";
    return roles;
}

Qt::ItemFlags BoundingBoxListModel::flags(const QModelIndex &index) const {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}
int BoundingBoxListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_ids.size();
}

QVariant BoundingBoxListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (index.row() >= m_ids.size() || !m_store) {
        return QVariant();
    }
    auto id = m_ids[index.row()];
    switch (role) {
    case IdRole: return id;
    case LabelRole: return m_presentor->getLabel(id);
    case RectRole: return QVariant::fromValue(m_presentor->getRect(id));
    default: return QVariant();
    }
}

int BoundingBoxListModel::getIndex(uint32_t id) {
    auto it = std::find(m_ids.begin(), m_ids.end(), id);
    if (it == m_ids.end())
        return -1;
    return static_cast<int>(std::distance(m_ids.begin(), it));
}
void BoundingBoxListModel::onBoxAdded(uint32_t id) {
    int index = m_ids.size();
    beginInsertRows(QModelIndex(), index, index);
    m_ids.push_back(id);
    endInsertRows();
}
void BoundingBoxListModel::onBoxRemoved(uint32_t id) {
    auto index = getIndex(id);
    if (index < 0) {
        return;
    }
    beginRemoveRows(QModelIndex(), index, index);
    m_ids.erase(m_ids.begin() + index);
    endRemoveRows();
}
void BoundingBoxListModel::onBoxUpdated(uint32_t id) {
    int row = getIndex(id);
    if (row < 0)
        return;
    const QModelIndex modelIndex = this->index(row);
    emit dataChanged(modelIndex, modelIndex, {RectRole});
}
// void BoundingBoxListModel::onFlagUpdated(const std::string &key) {
//     int row = getIndex(key);
//     if (row < 0)
//         return;
//     const QModelIndex modelIndex = this->index(row);
//     emit dataChanged(modelIndex, modelIndex, {FlagsRole});
// }
void BoundingBoxListModel::onClear() {
    beginResetModel();
    m_ids.clear();
    endResetModel();
}