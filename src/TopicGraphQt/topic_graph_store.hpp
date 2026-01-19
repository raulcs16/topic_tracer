#pragma once
#include "topic_graph.hpp"
#include "ui_states.hpp"
#include <QObject>
#include <QtQml/qqml.h>


class TGStore : public QObject, public ITopicGraphObserver {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Created by controller,used in QtListModels")

public:
    void onTopicAdded(const Topic &topic) override;
    void onTopicRemoved(uint32_t id) override;
    void onTopicRenamed(const Topic &topic) override;
    void onEdgeAdded(const Edge &edge) override;
    void onEdgeRemoved(const std::string &key) override;
    void onClear() override;

    QString label(uint32_t id);
    StateFlags flags(uint32_t id);
    StateFlags flags(const std::string &key);

    void setTopicState(uint32_t id, StateFlag flag, bool state);

    void setSelected(uint32_t id);

signals:
    void labelUpdated(uint32_t id);
    void flagUpdated(uint32_t id);
    // void flagUpdated(const std::string &key);

private:
    const QString m_notFound = "NotFound";
    std::unordered_map<uint32_t, QString> m_labels;
    std::unordered_map<uint32_t, ItemState> m_topicFlags;
    std::unordered_map<std::string, ItemState> m_edgeFlags;
};