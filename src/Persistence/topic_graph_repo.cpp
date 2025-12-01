#include "topic_graph_repo.hpp"
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>

QJsonDocument TopicGraphSerializer::toJson(const TopicGraph &graph) {

    QJsonObject root;
    root["version"] = 1;

    QJsonArray topicArray;
    for (const auto &t : graph.topics()) {
        topicArray.append(encodeTopic(*t));
    }
    root["topics"] = topicArray;
    QJsonArray edgesArray;
    for (const auto &e : graph.edges()) {
        edgesArray.append(encodeEdge(*e));
    }
    root["edges"] = edgesArray;
    return QJsonDocument(root);
}

bool TopicGraphSerializer::fromJson(const QJsonDocument &doc,
                                    TopicGraph &graph,
                                    QString *error) {
    return true;
}

QJsonObject TopicGraphSerializer::encodeTopic(const Topic &t) {
    QJsonObject topic;
    topic["id"] = static_cast<int>(t.id);
    topic["name"] = QString::fromStdString(t.name);
    topic["type"] = static_cast<int>(t.type);
    return topic;
}


QJsonObject TopicGraphSerializer::encodeEdge(const Edge &e) {
    QJsonObject edge;
    edge["key"] = QString::fromStdString(e.key);
    edge["from"] = static_cast<int>(e.from);
    edge["to"] = static_cast<int>(e.to);
    edge["type"] = static_cast<int>(e.type);
    return edge;
}

bool TopicGraphSerializer::decodeTopic(const QJsonObject &obj,
                                       TopicGraph &graph,
                                       QHash<int, Topic *> &idMap,
                                       QString *error) {
    return true;
}
bool TopicGraphSerializer::decodeEdge(const QJsonObject &obj,
                                      TopicGraph &graph,
                                      const QHash<int, Topic *> &idMap,
                                      QString *error) {
    return true;
}
TopicGraphRepository::TopicGraphRepository(QString basePath) : m_basePath(basePath) {}
bool TopicGraphRepository::save(const TopicGraph &graph,
                                QString fileName,
                                QString *error) {

    QFile file(m_basePath + "/" + fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        if (error)
            *error = "Cannot open file for writing";
        return false;
    }

    QJsonDocument doc = TopicGraphSerializer::toJson(graph);
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}
bool TopicGraphRepository::load(const TopicGraph &graph,
                                QString file_name,
                                QString *error) {
    return false;
}