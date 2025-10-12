#include "graph_controller.hpp"
#include "node_list_model.hpp"

NodeListModel::NodeListModel(GraphController *controller)
    : QAbstractListModel{controller}, m_controller{controller} {}

QHash<int, QByteArray> NodeListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "node";
    roles[XRole] = "posx";
    roles[YRole] = "posy";
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
    GraphNode nodeInfo = m_nodes[index.row()];

    switch (role) {
    case IdRole: return QVariant::fromValue(nodeInfo.id);
    case XRole: return nodeInfo.x;
    case YRole: return nodeInfo.y;
    default: return QVariant();
    }
}

void NodeListModel::resetNodes(const std::vector<GraphNode> &nodes) {
    beginResetModel();
    m_nodes.clear();
    m_nodes = nodes;
    endResetModel();
}
void NodeListModel::onGaphChanged() {
    beginResetModel();
    m_nodes.clear();
    endResetModel();
}