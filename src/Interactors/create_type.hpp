
#pragma once

#include "io_node_type_reg.hpp"
#include "layout_engine.hpp"
#include "node_type_registry.hpp"


struct CreateNodeTypeResponse {
    bool success;
    QString error;
    type_id id;
};

class CreateNodeType {
public:
    explicit CreateNodeType(NodeTypeRegistry &reg,
                            LayoutEngine &layout,
                            IONodeTypeReg &io)
        : m_io(io), m_layout(layout), m_reg(reg) {}
    CreateNodeTypeResponse execute(const QString &label) {
        auto type = m_reg.create(label);
        if (type == nullptr) {
            return {false, "error: registry unable to create " + label, 0};
        }
        m_layout.createPool(type->typeId);
        m_io.onTypeCreated(type->typeId);
        return {true, "new node type created!", type->typeId};
    }

private:
    IONodeTypeReg &m_io;
    NodeTypeRegistry &m_reg;
    LayoutEngine &m_layout;
};