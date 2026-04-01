#include "graph_store.hpp"

void GraphStore::onNodeAdded(const Node &node) {
    m_labels.emplace(node.id, QString::fromStdString(node.label));
    m_nodeFlags.emplace(node.id, StateFlag::None);
    m_nodePosition[node.id] = Pos{};
    m_nodeheats[node.id] = 0.0f;
    emit nodeAdded(node.id);
}
void GraphStore::onNodeRemoved(uint32_t id) {
    emit nodeDeleted(id);
    auto it = m_labels.find(id);
    if (it != m_labels.end())
        m_labels.erase(it);
    auto tt = m_nodeFlags.find(id);
    if (tt != m_nodeFlags.end())
        m_nodeFlags.erase(tt);
    auto pt = m_nodePosition.find(id);
    if (pt != m_nodePosition.end())
        m_nodePosition.erase(pt);
    auto ht = m_nodeheats.find(id);
    if (ht != m_nodeheats.end())
        m_nodeheats.erase(ht);
}
void GraphStore::onNodeRenamed(const Node &node) {
    auto it = m_labels.find(node.id);
    if (it == m_labels.end())
        return;
    it->second = QString::fromStdString(node.label);
    emit labelUpdated(node.id);
}
void GraphStore::onEdgeAdded(const Edge &edge) {
    m_edgeFlags.emplace(edge.key, StateFlag::None);
    m_edgeTypes.emplace(edge.key, edge.type);
    m_edgePosData[edge.key] = EdgePos();
    emit edgeAdded(edge.key);
}
void GraphStore::onEdgeRemoved(const std::string &key) {
    emit edgeDeleted(key);
    auto it = m_edgeFlags.find(key);
    if (it != m_edgeFlags.end())
        m_edgeFlags.erase(it);
    auto tt = m_edgeTypes.find(key);
    if (tt != m_edgeTypes.end())
        m_edgeTypes.erase(tt);
    auto pt = m_edgePosData.find(key);
    if (pt != m_edgePosData.end())
        m_edgePosData.erase(pt);
}
void GraphStore::onClear() {
    m_labels.clear();
    m_nodeFlags.clear();
    m_nodeheats.clear();
    m_nodePosition.clear();
    //edges
    m_edgeFlags.clear();
    m_edgeTypes.clear();
    m_edgePosData.clear();
    emit clear();
}

QString GraphStore::label(uint32_t id) {
    auto it = m_labels.find(id);
    if (it == m_labels.end()) {
        return "";
    }
    return it->second;
}
StateFlags GraphStore::flags(uint32_t id) {
    auto it = m_nodeFlags.find(id);
    if (it == m_nodeFlags.end()) {
        return {};
    }
    return it->second.flags;
}
QRectF GraphStore::rect(uint32_t clusterId) { return m_rects[clusterId]; }
StateFlags GraphStore::flags(const std::string &key) {
    auto it = m_edgeFlags.find(key);
    if (it == m_edgeFlags.end()) {
        return {};
    }
    return it->second.flags;
}
EdgeType GraphStore::edgeType(const std::string &key) {
    auto it = m_edgeTypes.find(key);
    if (it == m_edgeTypes.end()) {
        return EdgeType::Null;
    }
    return it->second;
}
void GraphStore::setNodeHeat(uint32_t id, float heat) {
    m_nodeheats[id] = heat;
    emit nodeHeatUpdated(id);
}
void GraphStore::setNodeState(uint32_t id, StateFlag flag, bool state) {
    auto it = m_nodeFlags.find(id);
    if (it == m_nodeFlags.end()) {
        return;
    }
    if (state) {
        it->second.add(flag);
    } else {
        if (it->second.has(flag))
            it->second.remove(flag);
    }
    emit nodeFlagUpdated(id);
}
void GraphStore::setEdgeState(const std::string &key, StateFlag flag, bool state) {
    auto it = m_edgeFlags.find(key);
    if (it == m_edgeFlags.end()) {
        return;
    }
    if (state) {
        it->second.add(flag);
    } else {
        if (it->second.has(flag))
            it->second.remove(flag);
    }
    emit edgeFlagUpdated(key);
}
void GraphStore::onGraphBluePrint(GraphBlueprint blueprint) {
    clear();
    for (const auto node : blueprint.isoNodes) {
        onNodeAdded(node);
    }
    size_t i = 1;
    for (const auto &cluster : blueprint.clusters) {
        for (const auto node : cluster.nodes) {
            onNodeAdded(node);
        }
        for (const auto &edge : cluster.edges) {
            onEdgeAdded(edge);
        }
    }
}
QString GraphStore::findMatch(QString pattern) {
    QString match{};

    for (const auto &[_, label] : m_labels) {
        if (label.startsWith(pattern)) {
            match = label;
            break;
        }
    }

    return match;
}
void GraphStore::clearPath() {
    for (auto &[id, state] : m_nodeFlags) {
        if (state.has(StateFlag::InPath)) {
            m_nodeFlags[id].remove(StateFlag::InPath);
        }
        m_nodeFlags[id].remove(StateFlag::Hidden);
        emit nodeFlagUpdated(id);
    }
    for (auto &[id, state] : m_edgeFlags) {
        if (state.has(StateFlag::InPath)) {
            m_edgeFlags[id].remove(StateFlag::InPath);
        }
        m_edgeFlags[id].remove(StateFlag::Hidden);
        emit edgeFlagUpdated(id);
    }
}
std::vector<uint32_t> GraphStore::setAllNodes(StateFlag flag, bool state) {
    std::vector<uint32_t> ids;
    for (auto &[id, _] : m_nodeFlags) {
        ids.push_back(id);
        setNodeState(id, flag, state);
    }
    return ids;
}

void GraphStore::onNodeAdded(const GraphNode &node) {}

Pos GraphStore::pos(uint32_t id) { return m_nodePosition[id]; }
float GraphStore::heat(uint32_t id) { return m_nodeheats[id]; }
void GraphStore::onNodeUpdated(const GraphNode &node) {
    m_nodePosition[node.id].x = node.x;
    m_nodePosition[node.id].y = node.y;
    emit nodePosUpdated(node.id);
}
void GraphStore::onEdgeAdded(const GraphEdge &edge) {}
void GraphStore::onEdgeUpdated(const GraphEdge &edge) {
    m_edgePosData[edge.key].source.x = edge.source_x;
    m_edgePosData[edge.key].source.y = edge.source_y;
    m_edgePosData[edge.key].target.x = edge.target_x;
    m_edgePosData[edge.key].target.y = edge.target_y;
    for (const auto &p : edge.bends) {
        Pos pos;
        pos.x = p.m_x;
        pos.y = p.m_y;
        m_edgePosData[edge.key].bends.push_back(pos);
    }
    emit edgePositionUpdated(edge.key);
}
EdgePos GraphStore::edgePos(const std::string &key) { return m_edgePosData[key]; }
void GraphStore::onClusterRectUpdated(uint32_t clusterId,
                                      float x,
                                      float y,
                                      float w,
                                      float h) {
    bool isNew = (m_rects.find(clusterId) == m_rects.end());
    m_rects[clusterId] = QRectF{x, y, w, h};
    if (isNew)
        emit boxAdded(clusterId);
    else
        emit boxUpdated(clusterId);
}
void GraphStore::onClusterRectDeleted(uint32_t clusterId) {
    emit boxDeleted(clusterId);
    auto it = m_rects.find(clusterId);
    if (it != m_rects.end())
        m_rects.erase(it);
}