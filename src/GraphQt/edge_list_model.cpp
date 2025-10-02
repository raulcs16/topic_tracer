// * Copyright (c) 2013 Christoph Schulz
#include "edge_list_model.hpp"
#include "graph.hpp"
#include <QPointF>
#include <QQmlInfo>


EdgeListModel::EdgeListModel(Graph *graph)
    : QAbstractListModel(graph), ogdf::GraphObserver(&graph->g()), m_graph(graph) {}

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

int EdgeListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return count();
}


QVariant EdgeListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    ogdf::edge e = m_edges[index.row()];
    switch (role) {
    case Qt::DisplayRole: return QVariant(m_graph->edge(e));
    case SourceRole: return QVariant(m_graph->node(e->source()));
    case SourceXRole: return QVariant(m_graph->attributes().x(e->source()));
    case SourceYRole: return QVariant(m_graph->attributes().y(e->source()));
    case TargetRole: return QVariant(m_graph->node(e->target()));
    case TargetXRole: return QVariant(m_graph->attributes().x(e->target()));
    case TargetYRole: return QVariant(m_graph->attributes().y(e->target()));
    case BendsRole: {
        QVariantList points;
        ogdf::DPolyline &bends = m_graph->attributes().bends(e);
        if (bends.size() == 0) {
            m_graph->attributes().addNodeCenter2Bends();
        }
        ogdf::DPolyline::const_iterator bend;
        for (bend = bends.begin(); bend != bends.end(); bend++) {
            points.append(QPointF((*bend).m_x, (*bend).m_y));
        }
        return points;
    }
    default: return QVariant();
    }
}
bool EdgeListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid())
        return false;

    ogdf::edge v = m_edges[index.row()];
    switch (role) {
    case SourceRole: break;
    case SourceXRole: break;
    case SourceYRole: break;
    case TargetRole: break;
    case TargetXRole: break;
    case TargetYRole: break;
    case BendsRole: break;
    default: return false;
    }

    QVector<int> roles;
    roles << SourceRole << SourceXRole << SourceYRole << TargetRole << TargetXRole
          << TargetYRole << BendsRole;
    emit dataChanged(index, index, roles);
    m_graph->layout()->invalidate();
    return true;
}
Qt::ItemFlags EdgeListModel::flags(const QModelIndex &index) const {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

void EdgeListModel::reInit() {
    beginResetModel();
    m_edges.clear();
    for (auto it = m_graph->g().edges.begin(); it != m_graph->g().edges.end(); ++it) {
        ogdf::edge e = *it;
        m_edges.append(e);
        m_graph->insertEdge(e, QString("%1").arg(e->index()));
    }
    endResetModel();
    emit countChanged();
}

void EdgeListModel::cleared() {
    beginResetModel();
    m_edges.clear();
    m_graph->clearEdges();
    endResetModel();
    emit countChanged();
}

void EdgeListModel::nodeAdded(ogdf::node v) {}

void EdgeListModel::nodeDeleted(ogdf::node v) {}

void EdgeListModel::edgeAdded(ogdf::edge e) {
    beginInsertRows(QModelIndex(), m_edges.count(), m_edges.count());
    m_edges.append(e);
    m_graph->insertEdge(e, QString("%1").arg(e->index()));
    endInsertRows();
    emit countChanged();
}

void EdgeListModel::edgeDeleted(ogdf::edge e) {
    int index = m_edges.indexOf(e);
    Q_ASSERT(index != -1);
    beginRemoveRows(QModelIndex(), index, index);
    m_edges.removeAt(index);
    m_graph->removeEdge(e);
    endRemoveRows();
    emit countChanged();
}

void EdgeListModel::attributesChanged() {
    QModelIndex top = createIndex(0, 0);
    QModelIndex bottom = createIndex(count(), 0);
    QVector<int> roles;
    roles << SourceRole << SourceXRole << SourceYRole << TargetRole << TargetXRole
          << TargetYRole << BendsRole;
    emit dataChanged(top, bottom, roles);
}

int EdgeListModel::count() const { return m_edges.count(); }

QString EdgeListModel::get(int index) const {
    ogdf::edge e = m_edges.value(index);
    if (e) {
        return m_graph->edge(e);
    } else {
        return QString();
    }
}

QString EdgeListModel::getSource(int index) const {
    ogdf::edge e = m_edges.value(index);
    if (e) {
        return m_graph->node(e->source());
    } else {
        return QString();
    }
}

QString EdgeListModel::getTarget(int index) const {
    ogdf::edge e = m_edges.value(index);
    if (e) {
        return m_graph->node(e->target());
    } else {
        return QString();
    }
}

void EdgeListModel::insert(const QString &edge,
                           const QString &source,
                           const QString &target) {
    ogdf::node v = m_graph->v(source);
    ogdf::node w = m_graph->v(target);
    if (!v) {
        qmlInfo(this) << "Source node \"" << source << "\" does not exist";
    } else if (!w) {
        qmlInfo(this) << "Target node \"" << target << "\" does not exist";
    } else {
        ogdf::edge e = m_graph->g().newEdge(v, w);
        m_graph->insertEdge(e, edge);
        m_graph->layout()->invalidate();
    }
}

void EdgeListModel::remove(const QString &edge) {
    ogdf::edge e = m_graph->e(edge);
    if (e) {
        m_graph->g().delEdge(e);
        m_graph->layout()->invalidate();
    }
}