#pragma once
#include "geometry.hpp"
#include <QMetaType>
#include <QPointF>
#include <QRectF>
#include <QVector>
#include <QtQml/qqml.h>

namespace qt {

inline QPointF QPointFfromTT(tt::Point p) { return QPointF{p.x, p.y}; }

struct Line {
    Q_GADGET
    QML_VALUE_TYPE(line)
    Q_PROPERTY(QPointF start MEMBER start)
    Q_PROPERTY(QPointF end MEMBER end)
    Q_PROPERTY(QVector<QPointF> bends MEMBER bends)
public:
    Line() = default;
    QPointF start;
    QPointF end;
    QVector<QPointF> bends;

    static Line fromTT(const tt::Line line) {
        Line l;
        QPointF start = QPointFfromTT(line.start);
        QPointF end = QPointFfromTT(line.end);
        QVector<QPointF> bends;
        for (const auto &p : line.bends) {
            bends.append(QPointFfromTT(p));
        }
        l.start = start;
        l.end = end;
        l.bends = bends;
        return l;
    }
};
inline QRectF QRectFromTT(tt::Rect r) { return QRectF{r.x, r.y, r.width, r.height}; }


} // namespace qt