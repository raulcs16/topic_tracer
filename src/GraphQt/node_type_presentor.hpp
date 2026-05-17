#pragma once

#include "io_node_type_reg.hpp"
#include "layout_engine.hpp"
#include "node_type_registry.hpp"
#include <QObject>
#include <QString>
#include <QtQml/qqml.h>
#include <vector>

class NodeTypePresentor : public QObject, public IONodeTypeReg {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Created by controller,used in QtListModels")
public:
    explicit NodeTypePresentor(std::shared_ptr<const NodeTypeRegistry> reg,
                               std::shared_ptr<const LayoutEngine> layout,
                               QObject *parent = nullptr);

    //IONodeTypeReg
    void onTypeCreated(type_id id) override {
        auto clusterId = m_layout->getClusterIdForType(id);
        emit onNewTypeCreated(clusterId);
    }

    //getters
    QString getLabel(cluster_id id);
    QRectF getRect(cluster_id id);

signals:
    void onNewTypeCreated(cluster_id id);

private:
    std::shared_ptr<const NodeTypeRegistry> m_reg;
    std::shared_ptr<const LayoutEngine> m_layout;
};