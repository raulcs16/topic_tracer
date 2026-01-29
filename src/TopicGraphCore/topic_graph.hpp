#pragma once


#include "topic_graph_types.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct ITopicGraphObserver {
    virtual ~ITopicGraphObserver() = default;

    virtual void onTopicAdded(const Topic &topic) {}
    virtual void onTopicRemoved(uint32_t id) {}
    virtual void onTopicRenamed(const Topic &topic) {}
    virtual void onEdgeAdded(const Edge &edge) {}
    virtual void onEdgeRemoved(const std::string &key) {}
    virtual void onClear() {}
    virtual void onGraphBluePrint(GraphBlueprint blueprint) {}
};

class TopicGraph {
public:
    ~TopicGraph();
    bool addTopic(const std::string &name);
    bool addTopic(uint32_t id, const std::string &name);

    bool renameTopic(const std::string &topic, const std::string &new_name);
    bool renameTopic(uint32_t id, const std::string &new_name);
    bool deleteTopic(const std::string &name);
    bool deleteTopic(uint32_t id);
    //getters
    const Topic *getTopic(uint32_t id) const;
    const Topic *getTopic(const std::string &name) const;
    std::vector<const Topic *> topics() const;

    //Edge API
    bool addEdge(uint32_t from, uint32_t to, EdgeType type);
    bool addEdge(const std::string &topicA, const std::string &topicB, EdgeType type);
    bool addEdge(Edge edge);
    bool addEdge(const Topic *a, const Topic *b, EdgeType type);

    bool removeEdge(uint32_t from, uint32_t to);
    bool removeEdge(const std::string &topicA, const std::string &topicB);
    bool hasEdge(const std::string &key);
    //gettters
    const Edge *getEdge(const std::string &key) const;
    const Edge *getEdge(uint32_t from, uint32_t to) const;

    std::vector<const Edge *> edges() const;
    std::vector<const Edge *> getOutEdges(uint32_t from) const;
    std::vector<const Edge *> getInEdges(uint32_t to) const;

    //Graph Data
    size_t edgeCount() const { return m_edges.size(); }
    size_t topicCount() const { return m_topics.size(); }

    std::vector<const Topic *> parentsOf(uint32_t);
    std::vector<const Topic *> childrenOf(uint32_t id);

    void clear();
    void addObserver(ITopicGraphObserver *observer);
    void removeObserver(ITopicGraphObserver *observer);

    void beginBatchLoad();
    void endBatchLoad();

private:
    uint32_t nextId();
    std::string makeKey(uint32_t from, uint32_t to);

    GraphBlueprint buildGraphBlueprint();


private:
    template <typename Func, typename... Args>
    void notify(Func memberFunc, Args &&...args);

private:
    std::unordered_map<uint32_t, Topic> m_topics;
    std::unordered_map<std::string, Edge> m_edges;
    std::vector<ITopicGraphObserver *> m_observers;
    std::unordered_map<uint32_t, std::vector<uint32_t>> m_adjOutMap;
    std::unordered_map<uint32_t, std::vector<uint32_t>> m_adjInMap;
    uint32_t m_id_ref = 1;
    bool m_isLoading = false;
};