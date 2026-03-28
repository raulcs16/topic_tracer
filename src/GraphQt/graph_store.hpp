#pragma once
#include "graph.hpp"
#include "ui_states.hpp"
#include <QObject>
#include <QtQml/qqml.h>


class GraphStore : public QObject, public IGraphObserver {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Created by controller,used in QtListModels")

public:
    void onNodeAdded(const Node &node) override;
    void onNodeRemoved(uint32_t id) override;
    void onNodeRenamed(const Node &node) override;
    void onEdgeAdded(const Edge &edge) override;
    void onEdgeRemoved(const std::string &key) override;
    void onClear() override;
    void onGraphBluePrint(GraphBlueprint blueprint) override;

    QString label(uint32_t id);
    StateFlags flags(uint32_t id);
    StateFlags flags(const std::string &key);

    void setNodeState(uint32_t id, StateFlag flag, bool state);
    void setEdgeState(const std::string &key, StateFlag flag, bool state);
    QString findMatch(QString pattern);
    EdgeType edgeType(const std::string &key);


signals:
    void labelUpdated(uint32_t id);
    void nodeFlagUpdated(uint32_t id);
    void nodeDeleted(uint32_t id);
    void edgeFlagUpdated(const std::string &key);
    void edgeTypeUpdated(const std::string &key);
    void clear();
    // void flagUpdated(const std::string &key);

private:
    const QString m_notFound = "NotFound";
    std::unordered_map<uint32_t, QString> m_labels;
    std::unordered_map<uint32_t, ItemState> m_nodeFlags;
    std::unordered_map<std::string, ItemState> m_edgeFlags;
    std::unordered_map<std::string, EdgeType> m_edgeTypes;
};