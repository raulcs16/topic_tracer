#pragma once
#include <QMetaType>
#include <QtQml/qqml.h>

struct Pos {
    Q_GADGET
    QML_VALUE_TYPE(pos)
    Q_PROPERTY(double x MEMBER x)
    Q_PROPERTY(double y MEMBER y)
public:
    Pos() = default;
    double x = 0;
    double y = 0;

    bool operator==(const Pos &other) const { return x == other.x && y == other.y; }
    bool operator!=(const Pos &other) const { return !(*this == other); }
};
