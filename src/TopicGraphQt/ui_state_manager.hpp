#pragma once

#include <QObject>
#include <QUrl>
#include <QtQml/qqml.h>
#include <unordered_map>

struct UIState {
    bool selectable = true;
    bool selected = false;
    bool highlighted = false;
    bool dimmed = false;
    bool visible = true;
};

class UIStateManager : public QObject {
    Q_OBJECT

public:
    explicit UIStateManager(QObject *parent = nullptr);
    std::shared_ptr<const UIState> state(uint32_t id);
    void setState(uint32_t id, UIState state);
    void setSelectedId(uint32_t id);
    void unsetSelected();

private:
    std::unordered_map<uint32_t, std::shared_ptr<UIState>> m_stateMap;
    int m_selectedId = -1;
};
