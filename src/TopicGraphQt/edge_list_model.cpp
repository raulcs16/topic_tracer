#include "edge_list_model.hpp"


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
    default: return QVariant();
    }
}
void EdgeListModel::resetEdges(const std::vector<EdgeItem> &edges) {
    beginResetModel();
    m_edges.clear();
    m_edges = edges;
    endResetModel();
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