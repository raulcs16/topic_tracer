#include "graph_store.hpp"

void GraphStore::onNodeAdded(const Node &node) {
    m_labels.emplace(node.id, QString::fromStdString(node.label));
    emit labelUpdated(node.id);
    m_nodeFlags.emplace(node.id, StateFlag::None);
}
void GraphStore::onNodeRemoved(uint32_t id) {
    auto it = m_labels.find(id);
    if (it == m_labels.end())
        return;
    m_labels.erase(it);
    auto tt = m_nodeFlags.find(id);
    if (tt == m_nodeFlags.end())
        return;
    m_nodeFlags.erase(tt);
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
}
void GraphStore::onEdgeRemoved(const std::string &key) {
    auto it = m_edgeFlags.find(key);
    if (it == m_edgeFlags.end())
        return;
    m_edgeFlags.erase(it);
}
void GraphStore::onClear() {
    m_labels.clear();
    m_nodeFlags.clear();
    m_edgeFlags.clear();
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
StateFlags GraphStore::flags(const std::string &key) {
    auto it = m_edgeFlags.find(key);
    if (it == m_edgeFlags.end()) {
        return {};
    }
    return it->second.flags;
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