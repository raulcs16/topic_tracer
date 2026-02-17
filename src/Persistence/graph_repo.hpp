#pragma once

#include "graph.hpp"
#include <QJsonDocument>
#include <QString>


class GraphSerializer {
public:
    static QJsonDocument toJson(const Graph &graph);
    static bool fromJson(const QJsonDocument &doc, Graph &graph);

private:
    static QJsonObject encodeNode(const Node &t);
    static QJsonObject encodeEdge(const Edge &e);

    static bool decodeNode(const QJsonObject &obj,
                           Graph &graph,
                           QHash<int, const Node *> &idMap);
    static bool decodeEdge(const QJsonObject &obj,
                           Graph &graph,
                           const QHash<int, const Node *> &idMap);
};

class GraphRepository {
public:
    GraphRepository(QString basePath);
    bool save(const Graph &graph, QString file_name);
    bool load(Graph &graph, QString file_name);

private:
    QString m_basePath;
};