#pragma once


#include "graph_types.hpp"
#include "igraph_observer.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>


class Graph {
public:
    ~Graph();
    bool addNode(const std::string &label);
    bool addNode(uint32_t id, const std::string &label);

    bool renameNode(const std::string &label, const std::string &new_label);
    bool renameNode(uint32_t id, const std::string &new_label);
    bool deleteNode(const std::string &label);
    bool deleteNode(uint32_t id);
    //getters
    const Node *getNode(uint32_t id) const;
    const Node *getNode(const std::string &name) const;
    std::vector<const Node *> nodes() const;
    //Edge API
    bool addEdge(uint32_t from, uint32_t to, EdgeType type);
    bool addEdge(const std::string &nodeA, const std::string &nodeB, EdgeType type);
    bool addEdge(Edge edge);
    bool addEdge(const Node *a, const Node *b, EdgeType type);

    bool removeEdge(uint32_t from, uint32_t to);
    bool removeEdge(const std::string &nodeA, const std::string &nodeB);
    bool hasEdge(const std::string &key);
    //gettters
    const Edge *getEdge(const std::string &key) const;
    const Edge *getEdge(uint32_t from, uint32_t to) const;

    std::vector<const Edge *> edges() const;
    std::vector<const Edge *> getOutEdges(uint32_t from) const;
    std::vector<const Edge *> getInEdges(uint32_t to) const;

    //Graph Data
    size_t edgeCount() const { return m_edges.size(); }
    size_t nodeCount() const { return m_nodes.size(); }

    std::vector<const Node *> parentsOf(uint32_t);
    std::vector<const Node *> childrenOf(uint32_t id);

    void clear();
    void addObserver(IGraphObserver *observer);
    void removeObserver(IGraphObserver *observer);

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
    std::unordered_map<uint32_t, Node> m_nodes;
    std::unordered_map<std::string, Edge> m_edges;
    std::vector<IGraphObserver *> m_observers;
    std::unordered_map<uint32_t, std::vector<uint32_t>> m_adjOutMap;
    std::unordered_map<uint32_t, std::vector<uint32_t>> m_adjInMap;
    uint32_t m_id_ref = 1;
    bool m_isLoading = false;
};