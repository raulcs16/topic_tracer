#include "topic_graph_store.hpp"

void TGStore::onTopicAdded(const Topic &topic) {
    m_labels.emplace(topic.id, QString::fromStdString(topic.name));
    emit labelUpdated(topic.id);
    m_topicFlags.emplace(topic.id, StateFlag::None);
}
void TGStore::onTopicRemoved(uint32_t id) {
    auto it = m_labels.find(id);
    if (it == m_labels.end())
        return;
    m_labels.erase(it);
}
void TGStore::onTopicRenamed(const Topic &topic) {}
void TGStore::onEdgeAdded(const Edge &edge) {}
void TGStore::onEdgeRemoved(const std::string &key) {}
void TGStore::onClear() {
    m_labels.clear();
    m_topicFlags.clear();
    m_edgeFlags.clear();
}

QString TGStore::label(uint32_t id) {
    auto it = m_labels.find(id);
    if (it == m_labels.end()) {
        return m_notFound;
    }
    return it->second;
}
StateFlags TGStore::flags(uint32_t id) {
    auto it = m_topicFlags.find(id);
    if (it == m_topicFlags.end()) {
        return {};
    }
    return it->second.flags;
}
StateFlags TGStore::flags(const std::string &key) {
    auto it = m_edgeFlags.find(key);
    if (it == m_edgeFlags.end()) {
        return {};
    }
    return it->second.flags;
}

void TGStore::setTopicState(uint32_t id, StateFlag flag, bool state) {
    auto it = m_topicFlags.find(id);
    if (it == m_topicFlags.end()) {
        return;
    }
    if (state) {
        it->second.add(flag);
    } else {
        if (it->second.has(flag))
            it->second.remove(flag);
    }
    // qDebug() << "TGStore::setTopicState" << id << "_" << (int)flag << "_" << state;
    emit flagUpdated(id);
}
void TGStore::setSelected(uint32_t id) {
    auto it = m_topicFlags.find(id);
    if (it == m_topicFlags.end()) {
        return;
    }
    return it->second.add(StateFlag::Selected);
    emit flagUpdated(id);
}
