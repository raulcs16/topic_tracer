#pragma once
#include "iedge_store.hpp"
#include <unordered_map>
#include <vector>

class EdgeStore : public IEdgeStore {
public:
    bool addEdge(const Edge &edge) override;
    bool removeEdge(const std::string &key) override;
    void clear() override;
    // ---- Queries ------------------------------------------------------
    const Edge *getEdge(const std::string &key) const override;
    std::vector<const Edge *> getAllEdges() const override;
    bool contains(const std::string &key) const override;
    std::vector<const Edge *> getEdgesFrom(uint32_t from) const override;
    std::vector<const Edge *> getEdgesTo(uint32_t to) const override;
    size_t size() const override;

    // ---- Listeners ----------------------------------------------------

    void addListener(IEdgeStoreListener *listener) override;
    void removeListener(IEdgeStoreListener *listener) override;


private:
    void notifyAdded(const Edge &edge);
    void notifyRemoved(const Edge &edge);
    void notifyClear();

private:
    std::unordered_map<std::string, Edge> m_edges;
    std::vector<IEdgeStoreListener *> m_listeners;
};