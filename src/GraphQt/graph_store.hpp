#pragma once
#include "edge_pos.hpp"
#include "graph.hpp"
#include "layout_types.hpp"
#include "pos.hpp"
#include "ui_states.hpp"
#include <QObject>
#include <QRectF>
#include <QtQml/qqml.h>
#include <vector>


class GraphStore : public QObject, public IGraphObserver, public ILayoutObserver {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Created by controller,used in QtListModels")

public:
    GraphStore(QObject *parent = nullptr) : QObject(parent) {}

    //IGraphObserver
    void onNodeAdded(const Node &node) override;
    void onNodeRemoved(uint32_t id) override;
    void onNodeRenamed(const Node &node) override;
    void onEdgeAdded(const Edge &edge) override;
    void onEdgeRemoved(const std::string &key) override;
    void onClear() override;
    void onGraphBluePrint(GraphBlueprint blueprint) override;
    //ILayoutObserver
    void onNodeAdded(const GraphNode &node) override;
    void onNodeUpdated(const GraphNode &node) override;
    void onEdgeAdded(const GraphEdge &edge) override;
    void onEdgeUpdated(const GraphEdge &edge) override;
    void onGlobalBoundsUpdated(float x, float y, float w, float h) override;
    void onClusterRectUpdated(uint32_t clusterId,
                              float x,
                              float y,
                              float w,
                              float h) override;
    void onClusterRectDeleted(uint32_t) override;
    void onNodeClusterChanged(uint32_t nodeId, uint32_t clusterId) override;

    //Getters::nodes
    QString label(uint32_t id);
    StateFlags flags(uint32_t id);
    Pos pos(uint32_t id);
    float heat(uint32_t id);
    //Getters::edges
    EdgeType edgeType(const std::string &key);
    EdgePos edgePos(const std::string &key);
    StateFlags flags(const std::string &key);
    //Getters::boxes
    QRectF rect(uint32_t clusterId);
    //setters
    void setHoveredState(uint32_t nodeId, bool state);
    void setNodeState(uint32_t id, StateFlag flag, bool state);
    void setEdgeState(const std::string &key, StateFlag flag, bool state);
    std::vector<uint32_t> setAllNodes(StateFlag flag, bool state);
    void setNodeHeat(uint32_t id, float heat = 0.0f);
    void setGlobalActiveBox();
    //find
    QString findMatch(QString pattern);

    void clearPath();


signals:

    //nodes
    void nodeAdded(uint32_t id);
    void nodeDeleted(uint32_t id);
    void labelUpdated(uint32_t id);
    void nodePosUpdated(uint32_t id);
    void nodeHeatUpdated(uint32_t id);
    void nodeFlagUpdated(uint32_t id);
    //edges
    void edgeAdded(const std::string &key);
    void edgeDeleted(const std::string &key);
    void edgeFlagUpdated(const std::string &key);
    void edgeTypeUpdated(const std::string &key);
    void edgePositionUpdated(const std::string &key);

    //bounding boxes
    void boxAdded(uint32_t id);
    void boxDeleted(uint32_t id);
    void boxUpdated(uint32_t id);
    void activeBoxChanged(QRectF box, double x, double y);
    //all
    void clear();

private:
    const QString m_notFound = "NotFound";
    //nodes
    std::unordered_map<uint32_t, QString> m_labels;
    std::unordered_map<uint32_t, ItemState> m_nodeFlags;
    std::unordered_map<uint32_t, Pos> m_nodePosition;
    std::unordered_map<uint32_t, float> m_nodeheats;
    //edges
    std::unordered_map<std::string, EdgeType> m_edgeTypes;
    std::unordered_map<std::string, ItemState> m_edgeFlags;
    std::unordered_map<std::string, EdgePos> m_edgePosData;
    //boxes
    int m_hoverId = -1;
    QRectF m_globalRect;
    std::unordered_map<uint32_t, QRectF> m_rects;
    std::unordered_map<uint32_t, uint32_t> m_nodeRectMap;
};