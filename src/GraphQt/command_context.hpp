#pragma once
#include "graph_repo.hpp"
#include "graph_store.hpp"
#include "layout_engine.hpp"
#include "ui_context.hpp"

struct CommandContext {
    Graph *graph;
    GraphRepository *repo;
    LayoutEngine *layout;
    UIContext *uiContext;
    GraphStore *store;
};