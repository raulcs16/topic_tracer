#include "node_list_model.hpp"

NodeListModel::NodeListModel(UIStateManager *uiManager, QObject *parent)
    : QAbstractListModel{parent}, m_uiManager{uiManager} {}

QHash<int, QByteArray> NodeListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[LabelRole] = "label";
    roles[XRole] = "posx";
    roles[YRole] = "posy";
    roles[HighlightRole] = "highlighted";
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
    case HighlightRole: {
        return m_uiManager &&
               m_uiManager->state(nodeInfo.id)->has(StateFlag::Highlighted);
    }
    default: return QVariant();
    }
}
void NodeListModel::onNodeStateChanged(int id) {
    int idx = getNodeIndex(id);
    if (idx < 0)
        return;
    QModelIndex modelIndex = this->index(idx);
    emit dataChanged(modelIndex, modelIndex, {HighlightRole});
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
    endResetModel();
}
void NodeListModel::onGaphChanged() {
    beginResetModel();
    m_nodes.clear();
    endResetModel();
}