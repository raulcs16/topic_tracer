#include "edge_store.hpp"


bool EdgeStore::addEdge(const Edge &edge) {
    auto [it, inserted] = m_edges.emplace(edge.key, edge);
    if (!inserted)
        return false;
    notifyAdded(edge);
    return true;
}
bool EdgeStore::removeEdge(const std::string &key) {
    auto it = m_edges.find(key);
    if (it == m_edges.end())
        return false;

    Edge removed = it->second;
    m_edges.erase(it);

    notifyRemoved(removed);
    return true;
}
const Edge *EdgeStore::getEdge(const std::string &key) const {
    auto it = m_edges.find(key);
    return (it != m_edges.end()) ? &it->second : nullptr;
}
std::vector<const Edge *> EdgeStore::getAllEdges() const {
    std::vector<const Edge *> result;
    result.reserve(m_edges.size());
    for (auto &kv : m_edges)
        result.push_back(&kv.second);
    return result;
}


bool EdgeStore::contains(const std::string &key) const { return m_edges.count(key); }
size_t EdgeStore::size() const { return m_edges.size(); }
void EdgeStore::addListener(IEdgeStoreListener *listener) {
    m_listeners.push_back(listener);
}
void EdgeStore::removeListener(IEdgeStoreListener *listener) {
    m_listeners.erase(std::remove(m_listeners.begin(), m_listeners.end(), listener),
                      m_listeners.end());
}
void EdgeStore::notifyAdded(const Edge &edge) {
    for (auto *l : m_listeners) {
        l->onEdgeAdded(edge);
    }
}
void EdgeStore::notifyRemoved(const Edge &edge) {
    for (auto *l : m_listeners) {
        l->onEdgeRemoved(edge);
    }
}
void EdgeStore::notifyClear() {
    for (auto *l : m_listeners) {
        l->onClear();
    }
}
std::vector<const Edge *> EdgeStore::getEdgesFrom(uint32_t from) const {
    std::vector<const Edge *> result;
    for (auto &[_, edge] : m_edges) {
        if (edge.from == from)
            result.push_back(&edge);
    }
    return result;
}
std::vector<const Edge *> EdgeStore::getEdgesTo(uint32_t to) const {
    std::vector<const Edge *> result;
    for (auto &[_, edge] : m_edges) {
        if (edge.from == to)
            result.push_back(&edge);
    }
    return result;
}
void EdgeStore::clear() { m_edges.clear(); }