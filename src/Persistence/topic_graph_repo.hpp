#pragma once

#include "topic_graph.hpp"
#include <QJsonDocument>
#include <QString>


class TopicGraphSerializer {
public:
    static QJsonDocument toJson(const TopicGraph &graph);
    static bool fromJson(const QJsonDocument &doc, TopicGraph &graph);

private:
    static QJsonObject encodeTopic(const Topic &t);
    static QJsonObject encodeEdge(const Edge &e);

    static bool decodeTopic(const QJsonObject &obj,
                            TopicGraph &graph,
                            QHash<int, const Topic *> &idMap);
    static bool decodeEdge(const QJsonObject &obj,
                           TopicGraph &graph,
                           const QHash<int, const Topic *> &idMap);
};

class TopicGraphRepository {
public:
    TopicGraphRepository(QString basePath);
    bool save(const TopicGraph &graph, QString file_name);
    bool load(TopicGraph &graph, QString file_name);

private:
    QString m_basePath;
};