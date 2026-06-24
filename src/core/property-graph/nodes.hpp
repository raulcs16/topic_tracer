#pragma once

#include "geometry.hpp"
#include "pgtypes.hpp"
#include <QString>
#include <unordered_map>


struct NodeType {
    type_id typeId;
    QString label;
};

struct NodeProperties {
    node_id nodeId;
    std::unordered_map<QString, QVariant> properties;
};

struct Node {
    node_id id;
    type_id typeId;
    QString name;
    tt::Point position;
};