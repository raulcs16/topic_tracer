#pragma once

#include <QObject>
#include <QtQml/qqml.h>

class ENUMS : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("EXPOSE ENUMS TO QML")
public:
    enum class EdgeType {
        Null,
        Composes,
        Associates,
        Aggregates,
        Injects,
        Implements,
    };
    enum class TopicType {
        Concept,
        Concrete,
    };
    enum class StateFlag : uint32_t {
        None = 0,
        Selectable = 1 << 0,
        Selected = 1 << 1,
        EditMode = 1 << 2,
        Hidden = 1 << 3,
        Hovered = 1 << 4,
        InPath = 1 << 5,
    };

    Q_ENUM(StateFlag);
    Q_ENUM(EdgeType);
    Q_ENUM(TopicType);
};