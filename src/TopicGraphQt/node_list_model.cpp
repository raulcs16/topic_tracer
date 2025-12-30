#include "graph_keys.hpp"
#include "node_list_model.hpp"

NodeListModel::NodeListModel(QObject *parent) : QAbstractListModel{parent} {}

QHash<int, QByteArray> NodeListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
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
    case LabelRole: return nodeInfo.label;
    case XRole: return nodeInfo.x;
    case YRole: return nodeInfo.y;
    case FlagsRole: {
        auto it = m_stateFlags.find(nodeInfo.id);
        if (it == m_stateFlags.end())
            return QVariant();
        return static_cast<int>(it->second.flags);
    }
    case HeatRole: return nodeInfo.heat;
    default: return QVariant();
    }
}

size_t NodeListModel::getIndex(uint32_t id) {
    size_t i = 0;
    while (i < rowCount()) {
        if (m_nodes[i].id == id)
            break;
        i++;
    }
    return i;
}

void NodeListModel::resetNodes(const std::vector<NodeItem> &nodes) {
    beginResetModel();
    m_nodes = nodes;
    m_stateFlags.clear();

    for (const auto &n : m_nodes) {
        m_stateFlags.insert({n.id, {}});
    }
    endResetModel();
}
void NodeListModel::onGaphChanged() {
    beginResetModel();
    m_nodes.clear();
    endResetModel();
}

void NodeListModel::setFlagsOnId(uint32_t id, StateFlag flags) {
    int index = getIndex(id);
    if (index >= m_nodes.size())
        return;
    m_stateFlags[id].add(flags);
    const QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex, modelIndex, {FlagsRole});
}
void NodeListModel::unSetFlagsOnId(uint32_t id, StateFlag flags) {
    int index = getIndex(id);
    if (index >= m_nodes.size())
        return;
    m_stateFlags[id].remove(flags);
    const QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex, modelIndex, {FlagsRole});
}
void NodeListModel::updateHeatScore(uint32_t id, int score) {
    int index = getIndex(id);
    if (index >= m_nodes.size())
        return;
    m_nodes[index].heat = score;
    const QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex, modelIndex, {HeatRole});
}

void NodeListModel::addItem(NodeItem item) {
    const int newIndex = m_nodes.size();
    beginInsertRows(QModelIndex(), newIndex, newIndex);
    m_nodes.push_back(item);
    endInsertRows();
}
void NodeListModel::updatePos(uint32_t id, double x, double y) {
    int index = getIndex(id);
    if (index >= m_nodes.size())
        return;
    m_nodes[index].x = x;
    m_nodes[index].y = y;
    const QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex, modelIndex, {XRole, YRole});
}
void NodeListModel::deleteNode(uint32_t id) {}
void NodeListModel::updateLabel(uint32_t id, const QString &name) {}

void NodeListModel::onNodeAdded(const GraphNode &node) {
    const int newIndex = m_nodes.size();
    beginInsertRows(QModelIndex(), newIndex, newIndex);
    m_nodes.push_back(
        NodeItem{.x = node.x, .y = node.y, .id = node.id, .label = "", .heat = 0});
    m_stateFlags[node.id] = {};
    endInsertRows();
}
void NodeListModel::onNodeRemoved(uint32_t id) {
    int index = getIndex(id);
    if (index >= m_nodes.size())
        return;
    beginRemoveRows(QModelIndex(), index, index);
    m_nodes.erase(m_nodes.begin() + index);
    endRemoveRows();
}
void NodeListModel::onEdgeAdded(const GraphEdge &edge) {}
void NodeListModel::onEdgeRemoved(const GraphEdge &edge) {}
void NodeListModel::onClear() {
    beginResetModel();
    m_nodes.clear();
    m_stateFlags.clear();
    endResetModel();
}