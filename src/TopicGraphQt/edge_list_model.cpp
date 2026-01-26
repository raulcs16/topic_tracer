#include "edge_list_model.hpp"
#include "graph_keys.hpp"

EdgeListModel::EdgeListModel(TGStore *store, QObject *parent)
    : QAbstractListModel{parent}, m_tgstore(store) {}

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
        if (m_tgstore) {
            return static_cast<int>(m_tgstore->flags(edge.key));
        }
        return 0;
    }
    default: return QVariant();
    }
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


void EdgeListModel::deleteEdge(const std::string &key) {
    size_t index = getIndex(key);
    if (index >= m_edges.size())
        return;
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

void EdgeListModel::onNodeAdded(const GraphNode &node) {}
void EdgeListModel::onNodeRemoved(uint32_t id) {}
void EdgeListModel::onEdgeAdded(const GraphEdge &edge) {
    int index = getIndex(edge.key);
    if (index < m_edges.size())
        return updatePos(index, edge);

    index = m_edges.size();
    beginInsertRows(QModelIndex(), index, index);
    m_edges.push_back(extract(edge));
    endInsertRows();
}
void EdgeListModel::onEdgeRemoved(const std::string &key) {
    int index = getIndex(key);
    qDebug() << "ELM:onEdgeRemoved:" << index;
    if (index >= m_edges.size())
        return;
    qDebug() << "ELM:onEdgeRemoved:" << key;
    beginRemoveRows(QModelIndex(), index, index);
    m_edges.erase(m_edges.begin() + index);
    endRemoveRows();
}
void EdgeListModel::onClear() {
    beginResetModel();
    m_edges.clear();
    endResetModel();
}
void EdgeListModel::updatePos(int index, const GraphEdge &edge) {
    auto curr = m_edges[index];
    if (curr.target_x == edge.target_x && curr.target_y == edge.target_y)
        if (curr.source_x == edge.source_x && curr.source_y == edge.source_y)
            return;
    EdgeItem item = extract(edge);
    m_edges[index].bends = item.bends;
    m_edges[index].source_x = item.source_x;
    m_edges[index].source_y = item.source_y;
    m_edges[index].target_x = item.target_x;
    m_edges[index].target_y = item.target_y;
    const QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex,
                     modelIndex,
                     {SourceXRole, SourceYRole, TargetXRole, TargetYRole, BendsRole});
}
EdgeItem EdgeListModel::extract(const GraphEdge &edge) {
    std::vector<QPointF> bends;
    bends.reserve(edge.bends.size());
    for (const auto &point : edge.bends) {
        bends.emplace_back(point.m_x, point.m_y);
    }
    return {.key = edge.key,
            .from = edge.from,
            .to = edge.to,
            .source_x = edge.source_x,
            .source_y = edge.source_y,
            .target_x = edge.target_x,
            .target_y = edge.target_y,
            .bends = bends};
}

void EdgeListModel::onFlagUpdated(const std::string &key) {
    int index = getIndex(key);
    if (index >= m_edges.size())
        return;
    const QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex, modelIndex, {FlagsRole});
}