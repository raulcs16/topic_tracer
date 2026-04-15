#pragma once
#include <QMeteType>
#include <QtQml/qqml.h>
#include <vector>

namespace visual {

struct Point {
    Q_GADGET
    QML_VALUE_TYPE(point)
    Q_PROPERTY(double x MEMBER x)
    Q_PROPERTY(double y MEMBER y)
public:
    Point() = default();
    double x = 0.0;
    double y = 0.0;
    bool operator==(const Point &other) const { return x == other.x && y == other.y; }
    bool operator!=(const Point &other) const { return !(*this == other); }
};

// Use a type alias for Path to keep it clean
using Path = std::vector<Point>;

struct Line {
    Q_GADGET
    QML_VALUE_TYPE(line)
    Q_PROPERTY(Point source MEMBER source)
    Q_PROPERTY(Point target MEMBER target)
    Q_PROPERTY(Path bends MEMBER bends)
public:
    Line() = default;
    Point start;
    Point end;
    Path bends;
};

} // namespace visual