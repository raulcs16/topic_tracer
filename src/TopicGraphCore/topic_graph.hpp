#pragma once


#include "iedge_store.hpp"
#include "itopic_store.hpp"
#include "topic_graph_types.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>

class TopicGraph {
public:
    explicit TopicGraph(ITopicStore &topicStore, IEdgeStore &edgeStore);
    ~TopicGraph() = default;
    const Topic *addTopic(const std::string &name);
    const Topic *addTopic(uint32_t id, const std::string &name);
    bool renameTopic(uint32_t id, const std::string &new_name);
    bool deleteTopic(uint32_t id);
    //getters
    const Topic *getTopic(uint32_t id) const;
    const Topic *getTopic(const std::string &name) const;
    std::vector<const Topic *> topics() const;

    //Edge API
    const Edge *addEdge(uint32_t from, uint32_t to, EdgeType type);
    const Edge *addEdge(const std::string &topicA,
                        const std::string &topicB,
                        EdgeType type);
    const Edge *addEdge(Edge edge);
    const Edge *addEdge(const Topic *a, const Topic *b, EdgeType type);

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

private:
    uint32_t nextId();
    std::string makeKey(uint32_t from, uint32_t to);

private:
    ITopicStore &m_topics;
    IEdgeStore &m_edges;
    std::unordered_map<uint32_t, std::vector<uint32_t>> m_adjOutMap;
    std::unordered_map<uint32_t, std::vector<uint32_t>> m_adjInMap;
    uint32_t m_id_ref = 1;
};