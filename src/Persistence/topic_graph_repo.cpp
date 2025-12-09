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
    if (!doc.isObject()) {
        if (error)
            *error = "json root not an object";
        return false;
    }
    QJsonObject root = doc.object();
    int version = root["version"].toInt(1);
    Q_UNUSED(version);

    QHash<int, const Topic *> idMap;
    QJsonArray topics = root["topics"].toArray();
    for (auto it : topics) {
        QJsonObject obj = it.toObject();
        if (!decodeTopic(obj, graph, idMap, error))
            return false;
    }
    QJsonArray edges = root["edges"].toArray();
    for (auto it : edges) {
        QJsonObject obj = it.toObject();
        if (!decodeEdge(obj, graph, idMap, error)) {
            return false;
        }
    }


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
                                       QHash<int, const Topic *> &idMap,
                                       QString *error) {
    if (!obj.contains("id") || !obj.contains("name") || !obj.contains("type")) {
        if (error)
            *error = "topic missing required fields";
        return false;
    }

    uint32_t id = obj["id"].toInt();
    std::string name = obj["name"].toString().toStdString();
    TopicType type = static_cast<TopicType>(obj["type"].toInt());

    const auto topic = graph.addTopic(id, name, type);
    if (!topic) {
        if (error)
            *error = "failed to add topic";
    }
    idMap[id] = topic;

    return true;
}
bool TopicGraphSerializer::decodeEdge(const QJsonObject &obj,
                                      TopicGraph &graph,
                                      const QHash<int, const Topic *> &idMap,
                                      QString *error) {
    if (!obj.contains("key") || !obj.contains("from") || !obj.contains("to") ||
        !obj.contains("type")) {
        if (error)
            *error = "Edge missing required fields";
        return false;
    }

    std::string key = obj["key"].toString().toStdString();
    uint32_t fromId = obj["from"].toInt();
    uint32_t toId = obj["to"].toInt();
    EdgeType type = static_cast<EdgeType>(obj["type"].toInt());

    if (!idMap.contains(fromId) || !idMap.contains(toId)) {
        if (error)
            *error = "Edge references invalid topic ID";
        return false;
    }

    if (!graph.addEdge(Edge{.key = key, .from = fromId, .to = toId, .type = type})) {
        if (error)
            *error = "Failed to create edge: " + QString::fromStdString(key);
        return false;
    }

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
bool TopicGraphRepository::load(TopicGraph &graph, QString file_name, QString *error) {
    QFile file(m_basePath + "/" + file_name);
    if (!file.open(QIODevice::ReadOnly)) {
        if (error)
            *error = "Cannot open file for reading";
        return false;
    }

    QJsonParseError parseErr;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseErr);

    if (parseErr.error != QJsonParseError::NoError) {
        if (error)
            *error = "JSON parse error: " + parseErr.errorString();
        return false;
    }

    return TopicGraphSerializer::fromJson(doc, graph, error);
}
