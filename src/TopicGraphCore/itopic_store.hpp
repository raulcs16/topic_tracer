#pragma once


#include "itopic_store_listener.hpp"
#include "topic_graph_types.hpp"
#include <optional>
#include <vector>

class ITopicStore {
public:
    virtual ~ITopicStore() = default;

    // ---- CRUD ---------------------------------------------------------

    virtual bool addTopic(const Topic &topic) = 0;
    virtual bool removeTopic(uint32_t id) = 0;
    virtual bool renameTopic(uint32_t id, const std::string &newName) = 0;
    virtual void clear() = 0;
    // ---- Queries ------------------------------------------------------
    virtual const Topic *getTopic(uint32_t id) const = 0;
    virtual const Topic *findTopic(const std::string &name) const = 0;
    virtual std::vector<const Topic *> getAllTopics() const = 0;
    virtual bool contains(uint32_t id) const = 0;
    virtual size_t size() const = 0;

    // ---- Listeners ----------------------------------------------------

    virtual void addListener(ITopicStoreListener *listener) = 0;
    virtual void removeListener(ITopicStoreListener *listener) = 0;
};
