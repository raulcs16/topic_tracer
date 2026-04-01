#pragma once
#include "pos.hpp"
#include <QMetaType>
#include <QtQml/qqml.h>


struct EdgePos {
    Q_GADGET
    QML_VALUE_TYPE(edgePos)
    Q_PROPERTY(Pos source MEMBER source)
    Q_PROPERTY(Pos target MEMBER target)
    Q_PROPERTY(QVector<Pos> bends MEMBER bends)
public:
    EdgePos() = default;
    Pos source;
    Pos target;
    QVector<Pos> bends;
};
