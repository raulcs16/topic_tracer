#include "rect_list_model.hpp"

RectListModel::RectListModel(QObject *parent) : QAbstractListModel{parent} {}

QHash<int, QByteArray> RectListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[RectId] = "rectId";
    roles[RectX] = "rectX";
    roles[RectY] = "rectY";
    roles[RectW] = "rectW";
    roles[RectH] = "rectH";
    return roles;
}

int RectListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_rects.size();
}

QVariant RectListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (index.row() >= m_rects.size()) {
        return QVariant();
    }
    RectData data = m_rects[index.row()];

    switch (role) {
    case RectId: return data.id;
    case RectX: return data.x;
    case RectY: return data.y;
    case RectH: return data.h;
    case RectW: return data.w;
    default: return QVariant();
    }
}

void RectListModel::onClusterRectUpdated(uint32_t id,
                                         float x,
                                         float y,
                                         float w,
                                         float h) {
    auto it = std::find_if(m_rects.begin(), m_rects.end(), [id](const RectData &r) {
        return r.id == id;
    });
    if (id == 0) {
        m_sceneBounds = QRectF(x, y, w, h);
        emit sceneBoundsChanged();
    } else if (it != m_rects.end()) {
        *it = {id, x, y, w, h};
        int row = std::distance(m_rects.begin(), it);
        emit dataChanged(index(row), index(row));
    } else {
        beginInsertRows(QModelIndex(), m_rects.size(), m_rects.size());
        m_rects.push_back({id, x, y, w, h});
        endInsertRows();
    }
}
void RectListModel::onClear() {
    beginResetModel();
    m_rects.clear();
    endResetModel();
}

void RectListModel::onClusterRectDeleted(uint32_t id) {
    auto it = std::find_if(m_rects.begin(), m_rects.end(), [id](const RectData &r) {
        return r.id == id;
    });
    if (it == m_rects.end()) {
        return;
    }
    int row = std::distance(m_rects.begin(), it);
    beginRemoveRows(QModelIndex(), row, row);
    m_rects.erase(it);
    endRemoveRows();
}

void RectListModel::setSceneBounds(BoundingBox box) {
    auto width = box.max_x - box.min_x;
    auto height = box.max_y - box.min_y;
    m_sceneBounds = QRectF(box.min_x, box.min_y, width, height);
    emit sceneBoundsChanged();
}