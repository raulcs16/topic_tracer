#pragma once

#include "graph_store.hpp"
#include "icommand.hpp"


class ClearPathCommand : public ICommand {
public:
    explicit ClearPathCommand(GraphStore *store) : m_store(store) {}
    CommandResult execute() override {
        m_store->clearPath();
        return CommandResult::ok("");
    }
    void undo() override {}

private:
    GraphStore *m_store;
};