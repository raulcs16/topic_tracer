#include "topic_store.hpp"


bool TopicStore::addTopic(const Topic &topic) {
    auto [it, inserted] = m_topics.emplace(topic.id, topic);
    if (!inserted)
        return false;
    notifyAdded(topic);
    return true;
}
bool TopicStore::removeTopic(uint32_t id) {
    auto it = m_topics.find(id);
    if (it == m_topics.end())
        return false;

    Topic removed = it->second;
    m_topics.erase(it);

    notifyRemoved(removed);
    return true;
}
bool TopicStore::renameTopic(uint32_t id, const std::string &newName) {
    auto it = m_topics.find(id);
    if (it == m_topics.end())
        return false;
    it->second.name = newName;
    notifyRenamed(it->second);
    return true;
}
const Topic *TopicStore::getTopic(uint32_t id) const {
    auto it = m_topics.find(id);
    return (it != m_topics.end()) ? &it->second : nullptr;
}
const Topic *TopicStore::findTopic(const std::string &name) const {
    for (const auto [_, topic] : m_topics) {
        if (topic.name == name) {
            return &topic;
        }
    }
    return nullptr;
}
std::vector<const Topic *> TopicStore::getAllTopics() const {
    std::vector<const Topic *> result;
    result.reserve(m_topics.size());
    for (auto &kv : m_topics)
        result.push_back(&kv.second);
    return result;
}


bool TopicStore::contains(uint32_t id) const { return m_topics.count(id); }
size_t TopicStore::size() const { return m_topics.size(); }
void TopicStore::addListener(ITopicStoreListener *listener) {
    m_listeners.push_back(listener);
}
void TopicStore::removeListener(ITopicStoreListener *listener) {
    m_listeners.erase(std::remove(m_listeners.begin(), m_listeners.end(), listener),
                      m_listeners.end());
}
void TopicStore::notifyAdded(const Topic &topic) {
    for (auto *l : m_listeners) {
        l->onTopicAdded(topic);
    }
}
void TopicStore::notifyRemoved(const Topic &topic) {
    for (auto *l : m_listeners) {
        l->onTopicRemoved(topic);
    }
}
void TopicStore::notifyRenamed(const Topic &topic) {
    for (auto *l : m_listeners) {
        l->onTopicRenamed(topic);
    }
}
void TopicStore::notifyClear() {
    for (auto *l : m_listeners) {
        l->onClear();
    }
}
void TopicStore::clear() {
    m_topics.clear();
    notifyClear();
}