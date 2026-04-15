#pragma once
#include "geometry.hpp"
#include <QMetaType>
#include <QRectF>
#include <QVector>
#include <QtQml/qqml.h>

namespace qt {

struct Point {
    Q_GADGET
    QML_VALUE_TYPE(point)
    Q_PROPERTY(double x MEMBER x)
    Q_PROPERTY(double y MEMBER y)
public:
    Point() = default;
    double x = 0.0;
    double y = 0.0;
    bool operator==(const Point &other) const { return x == other.x && y == other.y; }
    bool operator!=(const Point &other) const { return !(*this == other); }
    static Point fromTT(const tt::Point p) {
        Point point;
        point.x = p.x;
        point.y = p.y;
        return point;
    }
};

// Use a type alias for Path to keep it clean
using Path = QVector<Point>;

struct Line {
    Q_GADGET
    QML_VALUE_TYPE(line)
    Q_PROPERTY(Point start MEMBER start)
    Q_PROPERTY(Point end MEMBER end)
    Q_PROPERTY(Path bends MEMBER bends)
public:
    Line() = default;
    Point start;
    Point end;
    Path bends;

    static Line fromTT(const tt::Line line) {
        Line l;
        Point start = Point::fromTT(line.start);
        Point end = Point::fromTT(line.end);
        Path path;
        path.reserve(static_cast<int>(line.bends.size()));
        for (const auto &p : line.bends) {
            path.append(Point::fromTT(p));
        }
        l.start = start;
        l.end = end;
        l.bends = path;
        return l;
    }
};
inline QRectF QRectFromTT(tt::Rect r) { return QRectF{r.x, r.y, r.width, r.height}; }


} // namespace qt