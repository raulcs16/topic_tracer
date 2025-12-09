#pragma once
#include "itopic_store.hpp"
#include <unordered_map>
#include <vector>

class TopicStore : public ITopicStore {
public:
    bool addTopic(const Topic &topic) override;
    bool removeTopic(uint32_t id) override;
    bool renameTopic(uint32_t id, const std::string &newName) override;
    void clear() override;
    // ---- Queries ------------------------------------------------------
    const Topic *getTopic(uint32_t id) const override;
    const Topic *findTopic(const std::string &name) const override;
    std::vector<const Topic *> getAllTopics() const override;
    bool contains(uint32_t id) const override;
    size_t size() const override;

    // ---- Listeners ----------------------------------------------------

    void addListener(ITopicStoreListener *listener) override;
    void removeListener(ITopicStoreListener *listener) override;

private:
    void notifyAdded(const Topic &topic);
    void notifyRemoved(const Topic &topic);
    void notifyRenamed(const Topic &topic);
    void notifyClear();

private:
    std::unordered_map<uint32_t, Topic> m_topics;
    std::vector<ITopicStoreListener *> m_listeners;
};