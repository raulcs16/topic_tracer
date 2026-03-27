#pragma once
#include "graph_repo.hpp"
#include "graph_store.hpp"
#include "layout_engine.hpp"
#include "rect_list_model.hpp"

struct CommandContext {
    Graph *graph;
    GraphRepository *repo;
    LayoutEngine *layout;
    RectListModel *rects;
    GraphStore *store;
};