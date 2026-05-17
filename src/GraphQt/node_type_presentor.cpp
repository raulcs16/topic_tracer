#include "geometryqt.hpp"
#include "node_type_presentor.hpp"

NodeTypePresentor::NodeTypePresentor(std::shared_ptr<const NodeTypeRegistry> reg,
                                     std::shared_ptr<const LayoutEngine> layout,
                                     QObject *parent = nullptr)
    : QObject(parent), m_layout(layout), m_reg(reg) {}

QString NodeTypePresentor::getLabel(cluster_id id) {
    auto nodeId = m_layout->getNodeIdFromClusterId(id);
    auto nodeType = m_reg->get(nodeId);
    if (nodeType == nullptr) {
        return "";
    }
    return nodeType->label();
}
QRectF NodeTypePresentor::getRect(cluster_id id) {
    auto rect = m_layout->getRect(id);
    return qt::QRectFromTT(rect);
}