#pragma once

#include "create_node.hpp"
#include "create_type.hpp"
#include "default_keys.hpp"
#include "icommand.hpp"

class NodeCommand : public ICommand {
public:
    NodeCommand(CreateNode *createNode, CreateNodeType *createType)
        : ICommand{"node"}, m_createNode{createNode}, m_createType{createType} {
        m_signature.addShortFlag(
            't',
            Flag{.isBoolean = false, .providerKey = "graph.nodes.types"});
        m_signature.addPositionalArg(
            Argument{.name = "new_node", .providerKey = TerminalKeys::None});
        m_signature.addShortFlag(
            'l',
            Flag{.isBoolean = false, .providerKey = TerminalKeys::ValueFlag});
    }
    bool execute(const std::vector<Token> &tokens) override {
        auto map = m_signature.mapping(tokens);
        //node -l=[NewLabel]
        auto label = map.find("l");
        if (label != map.end()) {
            auto newType = label->second;
            m_createType->execute(newType);
            return true;
        }
        //node [newNode] -t=[label]
        auto newNode = map.find("new_node");
        auto t = map.find("t");
        if (newNode != map.end() && t != map.end()) {
            auto nodeName = newNode->second; //string
            auto type = t->second;           //string
            m_createNode->execute(type, nodeName);
        }
    }

private:
    CreateNode *m_createNode;
    CreateNodeType *m_createType;
};