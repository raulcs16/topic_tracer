#pragma once

#include "geometry.hpp"
#include "pgtypes.hpp"
#include <QString>
#include <set>
#include <unordered_map>


struct AllowedDir {
    type_id from;
    type_id to;
    bool operator<(const AllowedDir &other) const {
        if (from != other.from)
            return from < other.from;
        return to < other.to;
    }
    bool operator==(const AllowedDir &other) const {
        return from == other.from && to == other.to;
    }
};

struct EdgeType {
    type_id typeId;
    QString label;
    std::set<AllowedDir> allowed_dir;
    bool allowSelfLoops = false;
};

struct EdgeProperties {
    edge_id edgeId;
    std::unordered_map<QString, QVariant> properties;
};

struct Edge {
    edge_id id;
    type_id typeId;
    node_id from;
    node_id to;
    tt::Line line;
};