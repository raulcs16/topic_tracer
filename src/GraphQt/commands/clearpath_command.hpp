#pragma once

#include "graph_store.hpp"
#include "icommand.hpp"


class ClearPathCommand : public ICommand {
public:
    explicit ClearPathCommand(GraphStore *store) : m_store(store) {}
    void execute() override { m_store->clearPath(); }
    void undo() override {}

private:
    GraphStore *m_store;
};