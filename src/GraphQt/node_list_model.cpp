/*
 * Copyright (c) 2013 Christoph Schulz
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details: http://www.gnu.org/copyleft/lesser
 * 
 */

#include "graph.hpp"
#include "node_list_model.hpp"

NodeListModel::NodeListModel(Graph *graph)
    : QAbstractListModel(graph), ogdf::GraphObserver(&graph->g()), m_graph(graph) {}

QHash<int, QByteArray> NodeListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "node";
    roles[XRole] = "posx";
    roles[YRole] = "posy";
    roles[WidthRole] = "w";
    roles[HeightRole] = "h";
    return roles;
}

int NodeListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return count();
}

Qt::ItemFlags NodeListModel::flags(const QModelIndex &index) const {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QVariant NodeListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    ogdf::node v = m_nodes[index.row()];
    switch (role) {
    case Qt::DisplayRole: return QVariant(m_graph->node(v));
    case XRole: return QVariant(m_graph->attributes().x(v));
    case YRole: return QVariant(m_graph->attributes().y(v));
    case WidthRole: return QVariant(m_graph->attributes().width(v));
    case HeightRole: return QVariant(m_graph->attributes().height(v));
    default: return QVariant();
    }
}

bool NodeListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid())
        return false;

    ogdf::node v = m_nodes[index.row()];
    switch (role) {
    case XRole: {
        m_graph->attributes().x(v) = value.toDouble();
        break;
    }
    case YRole: {
        m_graph->attributes().y(v) = value.toDouble();
        break;
    }
    case WidthRole: {
        m_graph->attributes().width(v) = value.toDouble();
        break;
    }
    case HeightRole: {
        m_graph->attributes().height(v) = value.toDouble();
        break;
    }
    default: return false;
    }

    QVector<int> roles;
    roles << XRole << YRole << WidthRole << HeightRole;
    emit dataChanged(index, index, roles);
    m_graph->layout()->invalidate();
    return true;
}


void NodeListModel::reInit() {
    beginResetModel();
    m_nodes.clear();

    for (auto it = m_graph->g().nodes.begin(); it != m_graph->g().nodes.end(); ++it) {
        ogdf::node v = *it;
        m_nodes.append(v);
        m_graph->insertNode(v, QString("%1").arg(v->index()));
    }
    endResetModel();
    emit countChanged();
}

void NodeListModel::cleared() {
    beginResetModel();
    m_nodes.clear();
    m_graph->clearNodes();
    endResetModel();
    emit countChanged();
}

void NodeListModel::nodeAdded(ogdf::node v) {
    beginInsertRows(QModelIndex(), m_nodes.count(), m_nodes.count());
    m_nodes.append(v);
    m_graph->insertNode(v, QString("%1").arg(v->index()));
    endInsertRows();
    emit countChanged();
}
void NodeListModel::nodeDeleted(ogdf::node v) {
    int index = m_nodes.indexOf(v);
    Q_ASSERT(index != -1);
    beginRemoveRows(QModelIndex(), index, index);
    m_nodes.removeAt(index);
    m_graph->removeNode(v);
    endRemoveRows();
    emit countChanged();
}
void NodeListModel::edgeAdded(ogdf::edge e) {}

void NodeListModel::edgeDeleted(ogdf::edge e) {}

void NodeListModel::attributesChanged() {
    QModelIndex top = createIndex(0, 0);
    QModelIndex bottom = createIndex(count(), 0);
    QVector<int> roles;
    roles << XRole << YRole << WidthRole << HeightRole;
    emit dataChanged(top, bottom, roles);
}

int NodeListModel::count() const { return m_nodes.count(); }

QString NodeListModel::get(int index) const {
    ogdf::node v = m_nodes.value(index);
    if (v) {
        return m_graph->node(v);
    } else {
        return QString();
    }
}

void NodeListModel::insert(const QString &node) {
    ogdf::node v = m_graph->g().newNode();
    m_graph->insertNode(v, node);
    m_graph->layout()->invalidate();
}

void NodeListModel::remove(const QString &node) {
    ogdf::node v = m_graph->v(node);
    if (v) {
        m_graph->g().delNode(v);
        m_graph->layout()->invalidate();
    }
}