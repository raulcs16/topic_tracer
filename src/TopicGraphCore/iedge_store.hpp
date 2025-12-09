#pragma once
#include "iedge_store_listener.hpp"
#include "topic_graph_types.hpp"
#include <optional>
#include <string>
#include <vector>

class IEdgeStore {
public:
    virtual ~IEdgeStore() = default;

    // ---- CRUD ---------------------------------------------------------

    virtual bool addEdge(const Edge &edge) = 0;
    virtual bool removeEdge(const std::string &key) = 0;
    virtual void clear() = 0;

    // ---- Queries ------------------------------------------------------

    virtual const Edge *getEdge(const std::string &key) const = 0;
    virtual std::vector<const Edge *> getAllEdges() const = 0;
    virtual bool contains(const std::string &key) const = 0;

    // Graph helpers (optional but useful)
    virtual std::vector<const Edge *> getEdgesFrom(uint32_t from) const = 0;
    virtual std::vector<const Edge *> getEdgesTo(uint32_t to) const = 0;
    virtual size_t size() const = 0;

    // ---- Listeners ----------------------------------------------------
    virtual void addListener(IEdgeStoreListener *listener) = 0;
    virtual void removeListener(IEdgeStoreListener *listener) = 0;
};
