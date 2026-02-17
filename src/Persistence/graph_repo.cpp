#include "graph_repo.hpp"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>

QJsonDocument GraphSerializer::toJson(const Graph &graph) {

    QJsonObject root;
    root["version"] = 1;

    QJsonArray nodeArry;
    for (const auto &t : graph.nodes()) {
        nodeArry.append(encodeNode(*t));
    }
    root["nodes"] = nodeArry;
    QJsonArray edgesArray;
    for (const auto &e : graph.edges()) {
        edgesArray.append(encodeEdge(*e));
    }
    root["edges"] = edgesArray;
    return QJsonDocument(root);
}

bool GraphSerializer::fromJson(const QJsonDocument &doc, Graph &graph) {
    if (!doc.isObject()) {
        qDebug() << "json root not an object";
        return false;
    }
    QJsonObject root = doc.object();
    int version = root["version"].toInt(1);
    Q_UNUSED(version);

    QHash<int, const Node *> idMap;
    bool succes = true;
    QJsonArray topics = root["nodes"].toArray();
    for (auto it : topics) {
        QJsonObject obj = it.toObject();
        if (!decodeNode(obj, graph, idMap))
            succes = false;
    }
    QJsonArray edges = root["edges"].toArray();
    for (auto it : edges) {
        QJsonObject obj = it.toObject();
        if (!decodeEdge(obj, graph, idMap)) {
            succes = false;
        }
    }

    return succes;
}

QJsonObject GraphSerializer::encodeNode(const Node &t) {
    QJsonObject node;
    node["id"] = static_cast<int>(t.id);
    node["label"] = QString::fromStdString(t.label);
    return node;
}


QJsonObject GraphSerializer::encodeEdge(const Edge &e) {
    QJsonObject edge;
    edge["key"] = QString::fromStdString(e.key);
    edge["from"] = static_cast<int>(e.from);
    edge["to"] = static_cast<int>(e.to);
    edge["type"] = static_cast<int>(e.type);
    return edge;
}

bool GraphSerializer::decodeNode(const QJsonObject &obj,
                                 Graph &graph,
                                 QHash<int, const Node *> &idMap) {
    if (!obj.contains("id") || !obj.contains("label")) {
        qDebug() << "node missing required fields";
        return false;
    }

    uint32_t id = static_cast<uint32_t>(obj["id"].toInt());
    std::string name = obj["label"].toString().toStdString();
    bool success = graph.addNode(id, name);
    if (!success) {
        qDebug() << "failed to add node";
    }
    auto node = graph.getNode(id);
    if (!node) {
        qDebug() << "failed to fetch node";
    }
    idMap[id] = node;

    return true;
}
bool GraphSerializer::decodeEdge(const QJsonObject &obj,
                                 Graph &graph,
                                 const QHash<int, const Node *> &idMap) {
    if (!obj.contains("key") || !obj.contains("from") || !obj.contains("to") ||
        !obj.contains("type")) {
        // qDebug() << "TGS::decodeEdge::Edge missing required fields";
        return false;
    }

    std::string key = obj["key"].toString().toStdString();
    if (key.length() == 0)
        return false;
    uint32_t fromId = static_cast<uint32_t>(obj["from"].toInt());
    uint32_t toId = static_cast<uint32_t>(obj["to"].toInt());
    EdgeType type = static_cast<EdgeType>(obj["type"].toInt());
    // qDebug() << "TGR::decodeEdge::edge=" << key << "type=" << static_cast<int>(type);

    if (!idMap.contains(fromId) || !idMap.contains(toId)) {
        // qDebug() << "TGR::decodeEdge::Edge references invalid node ID"
        // << "from=" << fromId << "," << "to=" << toId;
        return false;
    }

    if (!graph.addEdge(Edge{.key = key, .from = fromId, .to = toId, .type = type})) {
        // qDebug() << "TGR::decodeEdge::Failed to create edge: " +
        QString::fromStdString(key);
        return false;
    }

    return true;
}
GraphRepository::GraphRepository(QString basePath) : m_basePath(basePath) {
    QDir dir(m_basePath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}
bool GraphRepository::save(const Graph &graph, QString fileName) {

    QFile file(m_basePath + "/" + fileName + ".json");
    if (!file.open(QIODevice::WriteOnly)) {
        // qDebug() << "TGR::save::open file error";
        return false;
    }

    QJsonDocument doc = GraphSerializer::toJson(graph);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}
bool GraphRepository::load(Graph &graph, QString file_name) {

    QFile file(m_basePath + "/" + file_name + ".json");
    if (!file.open(QIODevice::ReadOnly)) {
        // qDebug() << "TGR::load::Cannot open file for reading";
        return false;
    }
    QJsonParseError parseErr;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseErr);
    if (parseErr.error != QJsonParseError::NoError) {
        // qDebug() << "TGR::load::JSON PArseError";
        return false;
    }
    file.close();
    return GraphSerializer::fromJson(doc, graph);
}
