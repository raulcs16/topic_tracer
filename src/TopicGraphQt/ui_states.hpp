#pragma once

#include <QFlags>
#include <QObject>
#include <QUrl>
#include <QtQml/qqml.h>
#include <unordered_map>

enum class StateFlag : uint32_t {
    None = 0,
    Selectable = 1 << 0,
    Selected = 1 << 1,
    Highlighted = 1 << 2,
    Dimmed = 1 << 3,
    Hidden = 1 << 4,
    Hovered = 1 << 5,
};

Q_DECLARE_FLAGS(StateFlags, StateFlag);
Q_DECLARE_OPERATORS_FOR_FLAGS(StateFlags);

struct ItemState {
    StateFlags flags = StateFlag::None;

    bool has(StateFlag f) const { return flags.testFlag(f); }
    void add(StateFlag f) { flags |= f; }
    void remove(StateFlag f) { flags &= ~StateFlags(f); }
    void toggle(StateFlag f) { flags ^= f; }
    void clear() { flags = StateFlag::None; }
};

class ENUMS : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("EXPOSE FLAGS TO QML")
public:
    enum class StateFlag : uint32_t {
        None = 0,
        Selectable = 1 << 0,
        Selected = 1 << 1,
        Highlighted = 1 << 2,
        Dimmed = 1 << 3,
        Hidden = 1 << 4,
        Hovered = 1 << 5,
    };

    Q_ENUM(StateFlag);
};