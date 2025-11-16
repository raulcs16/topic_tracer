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
    default: return QVariant();
    }
}
void NodeListModel::onNodeStateChanged(const std::string &id, const StateFlags &flags) {
    int idx = getNodeIndex(std::stoi(id));
    if (idx < 0)
        return;
    QModelIndex modelIndex = this->index(idx);

    QVector<int> roles = {};

    if (roles.empty())
        return;

    emit dataChanged(modelIndex, modelIndex, roles);
}
int NodeListModel::getNodeIndex(int id) {
    for (int i = 0; i < m_nodes.size(); i++) {
        if (m_nodes[i].id == id)
            return i;
    }
    return -1;
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
    int index = 0;
    while (index < m_nodes.size()) {
        if (m_nodes[index].id == id)
            break;
        index++;
    }
    if (index == m_nodes.size())
        return;
    m_stateFlags[id].add(flags);
    const QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex, modelIndex, {FlagsRole});
}
void NodeListModel::unSetFlagsOnId(uint32_t id, StateFlag flags) {
    int index = 0;
    while (index < m_nodes.size()) {
        if (m_nodes[index].id == id)
            break;
        index++;
    }
    if (index == m_nodes.size())
        return;
    m_stateFlags[id].remove(flags);
    const QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex, modelIndex, {FlagsRole});
}