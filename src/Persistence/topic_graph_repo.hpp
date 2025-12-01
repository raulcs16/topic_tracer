#pragma once

#include "topic_graph.hpp"
#include <QJsonDocument>
#include <QString>


class TopicGraphSerializer {
public:
    static QJsonDocument toJson(const TopicGraph &graph);
    static bool fromJson(const QJsonDocument &doc,
                         TopicGraph &graph,
                         QString *error = nullptr);

private:
    static QJsonObject encodeTopic(const Topic &t);
    static QJsonObject encodeEdge(const Edge &e);

    static bool decodeTopic(const QJsonObject &obj,
                            TopicGraph &graph,
                            QHash<int, Topic *> &idMap,
                            QString *error);
    static bool decodeEdge(const QJsonObject &obj,
                           TopicGraph &graph,
                           const QHash<int, Topic *> &idMap,
                           QString *error);
};

class TopicGraphRepository {
public:
    TopicGraphRepository(QString basePath);
    bool save(const TopicGraph &graph, QString file_name, QString *error = nullptr);
    bool load(const TopicGraph &graph, QString file_name, QString *error = nullptr);

private:
    QString m_basePath;
};