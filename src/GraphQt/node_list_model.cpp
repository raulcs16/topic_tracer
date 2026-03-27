#include "graph_keys.hpp"
#include "node_list_model.hpp"

NodeListModel::NodeListModel(GraphStore *store, QObject *parent)
    : QAbstractListModel{parent}, m_store(store) {

    connect(m_store, &GraphStore::labelUpdated, this, &NodeListModel::onLabelUpdated);
    connect(m_store, &GraphStore::nodeFlagUpdated, this, &NodeListModel::onFlagsUpdated);
}

QHash<int, QByteArray> NodeListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "nodeId";
    roles[LabelRole] = "label";
    roles[XRole] = "posx";
    roles[YRole] = "posy";
    roles[FlagsRole] = "flags";
    roles[HeatRole] = "heatScore";
    return roles;
}

int NodeListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_nodes.size();
}

Qt::ItemFlags NodeListModel::flags(const QModelIndex &index) const {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QVariant NodeListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (index.row() >= m_nodes.size()) {
        return QVariant();
    }
    NodeItem nodeInfo = m_nodes[index.row()];

    switch (role) {
    case IdRole: return QVariant::fromValue(nodeInfo.id);
    case LabelRole: {
        if (m_store) {
            return m_store->label(nodeInfo.id);
        }
        return QVariant();
    }
    case XRole: return nodeInfo.x;
    case YRole: return nodeInfo.y;
    case FlagsRole: {
        if (m_store) {
            return static_cast<int>(m_store->flags(nodeInfo.id));
        }
        return 0;
    }
    case HeatRole: return nodeInfo.heat;
    default: return QVariant();
    }
}

int NodeListModel::getIndex(uint32_t id) {
    auto it = m_idToRow.find(id);
    if (it == m_idToRow.end()) {
        return -1;
    }
    return it.value();
}


void NodeListModel::updateHeatScore(uint32_t id, float score) {
    int index = getIndex(id);
    if (index < 0)
        return;
    if (m_nodes[index].heat == score) {
        return;
    }
    m_nodes[index].heat = score;
    const QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex, modelIndex, {HeatRole});
}


void NodeListModel::updatePos(int index, double x, double y) {
    auto node = m_nodes[index];
    if (node.x == x && node.y == y)
        return;
    m_nodes[index].x = x;
    m_nodes[index].y = y;
    const QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex, modelIndex, {XRole, YRole});
}

void NodeListModel::onNodeUpdated(const GraphNode &node) {
    int index = getIndex(node.id);
    if (index < 0)
        return;
    return updatePos(index, node.x, node.y);
}
void NodeListModel::onNodeAdded(const GraphNode &node) {
    int index = getIndex(node.id);
    if (index >= 0) {
        updatePos(index, node.x, node.y);
        return;
    }
    index = m_nodes.size();
    beginInsertRows(QModelIndex(), index, index);
    m_nodes.push_back(NodeItem{.x = node.x, .y = node.y, .id = node.id, .heat = 0});
    m_idToRow.insert(node.id, index);
    endInsertRows();
}
void NodeListModel::onNodeRemoved(uint32_t id) {
    int index = getIndex(id);
    if (index < 0)
        return;
    beginRemoveRows(QModelIndex(), index, index);
    m_nodes.erase(m_nodes.begin() + index);
    auto it = m_idToRow.find(id);
    if (it != m_idToRow.end()) {
        m_idToRow.erase(it);
    }
    endRemoveRows();
}

void NodeListModel::onClear() {
    beginResetModel();
    m_nodes.clear();
    m_idToRow.clear();
    endResetModel();
}

void NodeListModel::onLabelUpdated(uint32_t id) {
    int row = getIndex(id);
    if (row < 0) {
        return; // Node doesn't exist in our list yet
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
