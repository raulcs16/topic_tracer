#include "topic_graph_repo.hpp"
#include <QDir>
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
        qDebug() << "json root not an object";
        return false;
    }
    QJsonObject root = doc.object();
    int version = root["version"].toInt(1);
    Q_UNUSED(version);

    QHash<int, const Topic *> idMap;
    bool succes = true;
    QJsonArray topics = root["topics"].toArray();
    for (auto it : topics) {
        QJsonObject obj = it.toObject();
        if (!decodeTopic(obj, graph, idMap, error))
            succes = false;
    }
    QJsonArray edges = root["edges"].toArray();
    for (auto it : edges) {
        QJsonObject obj = it.toObject();
        if (!decodeEdge(obj, graph, idMap, error)) {
            succes = false;
        }
    }
    // qDebug() << "TGR::load::graph state::\n";
    // for (const auto topics : graph.topics()) {
    //     qDebug() << topics->id << ":" << topics->name;
    // }

    return succes;
}

QJsonObject TopicGraphSerializer::encodeTopic(const Topic &t) {
    QJsonObject topic;
    topic["id"] = static_cast<int>(t.id);
    topic["name"] = QString::fromStdString(t.name);
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
    if (!obj.contains("id") || !obj.contains("name")) {
        qDebug() << "topic missing required fields";
        return false;
    }

    uint32_t id = obj["id"].toInt();
    std::string name = obj["name"].toString().toStdString();
    const auto topic = graph.addTopic(id, name);
    if (!topic) {
        qDebug() << "failed to add topic";
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
        qDebug() << "TGS::decodeEdge::Edge missing required fields";
        return false;
    }

    std::string key = obj["key"].toString().toStdString();
    if (key.length() == 0)
        return false;
    uint32_t fromId = obj["from"].toInt();
    uint32_t toId = obj["to"].toInt();
    EdgeType type = static_cast<EdgeType>(obj["type"].toInt());

    if (!idMap.contains(fromId) || !idMap.contains(toId)) {
        qDebug() << "TGR::decodeEdge::Edge references invalid topic ID"
                 << "from=" << fromId << "," << "to=" << toId;
        return false;
    }

    if (!graph.addEdge(Edge{.key = key, .from = fromId, .to = toId, .type = type})) {
        qDebug() << "TGR::decodeEdge::Failed to create edge: " +
                        QString::fromStdString(key);
        return false;
    }

    return true;
}
TopicGraphRepository::TopicGraphRepository(QString basePath) : m_basePath(basePath) {
    QDir dir(m_basePath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}
bool TopicGraphRepository::save(const TopicGraph &graph,
                                QString fileName,
                                QString *error) {

    QFile file(m_basePath + "/" + fileName + ".json");
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "TGR::save::open file error";
        return false;
    }

    QJsonDocument doc = TopicGraphSerializer::toJson(graph);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}
bool TopicGraphRepository::load(TopicGraph &graph, QString file_name, QString *error) {

    QFile file(m_basePath + "/" + file_name + ".json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "TGR::load::Cannot open file for reading";
        return false;
    }

    QJsonParseError parseErr;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseErr);

    if (parseErr.error != QJsonParseError::NoError) {
        qDebug() << "TGR::load::JSON PArseError";
        return false;
    }
    file.close();
    return TopicGraphSerializer::fromJson(doc, graph, error);
}
