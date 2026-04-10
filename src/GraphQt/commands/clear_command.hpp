#pragma once

#include "graph_store.hpp"
#include "icommand.hpp"


class ClearCommand : public ICommand {
public:
    explicit ClearCommand(GraphStore *store) : m_store(store) {}

    CommandResult execute() override {
        m_store->clear();
        return CommandResult::ok("");
    }
    void undo() override {}

private:
    GraphStore *m_store;
};