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
};

Q_DECLARE_FLAGS(StateFlags, StateFlag);
Q_DECLARE_OPERATORS_FOR_FLAGS(StateFlags);

struct UIState {
    StateFlags flags = StateFlag::None;
    bool has(StateFlag f) const { return flags.testFlag(f); }
};

class UIStateManager : public QObject {
    Q_OBJECT

public:
    explicit UIStateManager(QObject *parent = nullptr);
    std::shared_ptr<const UIState> state(uint32_t id) const;
    void setState(uint32_t id, StateFlags flags);
    void addState(uint32_t id, StateFlags flags);
    void removeState(uint32_t id, StateFlags flags);
    void setSelectedId(uint32_t id);
public slots:

    void setHoveredId(int id);

signals:
    void stateChanged(uint32_t id);

private:
    std::unordered_map<uint32_t, std::shared_ptr<UIState>> m_stateMap;
    int m_selectedId = -1;
    int m_hoveredId = -1;
};
