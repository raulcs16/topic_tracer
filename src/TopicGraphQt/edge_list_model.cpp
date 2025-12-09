#include "edge_list_model.hpp"
#include "graph_keys.hpp"

EdgeListModel::EdgeListModel(QObject *parent) : QAbstractListModel{parent} {}

QHash<int, QByteArray> EdgeListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "edge";
    roles[SourceRole] = "source";
    roles[SourceXRole] = "sourceX";
    roles[SourceYRole] = "sourceY";
    roles[TargetRole] = "target";
    roles[TargetXRole] = "targetX";
    roles[TargetYRole] = "targetY";
    roles[BendsRole] = "bends";
    roles[FlagsRole] = "flags";
    return roles;
}

Qt::ItemFlags EdgeListModel::flags(const QModelIndex &index) const {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}
int EdgeListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_edges.size();
}


QVariant EdgeListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (index.row() >= m_edges.size()) {
        return QVariant();
    }
    EdgeItem edge = m_edges[index.row()];
    switch (role) {
    case Qt::DisplayRole: return QVariant();
    case SourceRole: return QVariant(edge.from);
    case SourceXRole: return QVariant(edge.source_x);
    case SourceYRole: return QVariant(edge.source_y);
    case TargetRole: return QVariant(edge.to);
    case TargetXRole: return QVariant(edge.target_x);
    case TargetYRole: return QVariant(edge.target_y);
    case BendsRole: {
        QVariantList points;
        for (const auto &p : edge.bends)
            points.append(p);
        return points;
    }
    case FlagsRole: {
        auto it = m_stateFlags.find(edge.key);
        if (it == m_stateFlags.end())
            return QVariant();
        return static_cast<int>(it->second.flags);
    }
    default: return QVariant();
    }
}
void EdgeListModel::resetEdges(const std::vector<EdgeItem> &edges) {
    beginResetModel();
    m_edges.clear();
    m_stateFlags.clear();
    m_edges = edges;
    endResetModel();
}
size_t EdgeListModel::getIndex(const std::string &key) {
    size_t index = 0;
    while (index < m_edges.size()) {
        if (m_edges[index].key == key)
            break;
        index++;
    }

    return index;
}
// bool EdgeListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
//     if (!index.isValid())
//         return false;

//     ogdf::edge v = m_edges[index.row()];
//     switch (role) {
//     case SourceRole: break;
//     case SourceXRole: break;
//     case SourceYRole: break;
//     case TargetRole: break;
//     case TargetXRole: break;
//     case TargetYRole: break;
//     case BendsRole: break;
//     default: return false;
//     }

//     QVector<int> roles;
//     roles << SourceRole << SourceXRole << SourceYRole << TargetRole << TargetXRole
//           << TargetYRole << BendsRole;
//     emit dataChanged(index, index, roles);
//     m_graph->layout()->invalidate();
//     return true;
// }

void EdgeListModel::setFlagsOnId(const std::string &key, StateFlag flags) {
    int index = 0;
    while (index < m_edges.size()) {
        if (m_edges[index].key == key)
            break;
        index++;
    }
    if (index == m_edges.size())
        return;
    m_stateFlags[key].add(flags);
    const QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex, modelIndex, {FlagsRole});
}
void EdgeListModel::unSetFlagsOnId(const std::string &key, StateFlag flags) {
    size_t index = getIndex(key);
    if (index >= m_edges.size())
        return;
    m_stateFlags[key].remove(flags);
    const QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex, modelIndex, {FlagsRole});
}
void EdgeListModel::deleteEdge(const std::string &key) {
    size_t index = getIndex(key);
    if (index >= m_edges.size())
        return;
    m_stateFlags.erase(key);
    beginRemoveRows(QModelIndex(), index, index);
    m_edges.erase(m_edges.begin() + index);
    endRemoveRows();
}
void EdgeListModel::addItem(EdgeItem item) {

    size_t index = getIndex(item.key);
    if (index >= m_edges.size()) {

        const int newIndex = m_edges.size();
        beginInsertRows(QModelIndex(), newIndex, newIndex);
        m_edges.push_back(item);
        m_stateFlags[item.key] = {};
        endInsertRows();
    }
    m_edges[index].bends = item.bends;
    m_edges[index].source_x = item.source_x;
    m_edges[index].source_y = item.source_y;
    m_edges[index].target_x = item.target_x;
    m_edges[index].target_y = item.target_y;
    const QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex,
                     modelIndex,
                     {BendsRole, SourceXRole, SourceXRole, TargetXRole, TargetYRole});
}