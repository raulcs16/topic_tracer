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
    std::shared_ptr<const UIState> state(const std::string &id) const;
    void setState(const std::string &id, StateFlags flags);
    void addState(const std::string &id, StateFlags flags);
    void removeState(const std::string &id, StateFlags flags);
    void setSelectedId(const std::string &id);
public slots:

    void setHoveredId(const std::string &id);

signals:
    void stateChanged(const std::string &id);

private:
    std::unordered_map<std::string, std::shared_ptr<UIState>> m_stateMap;
    std::string m_selectedId{};
    std::string m_hoveredId{};
};
