#pragma once

#include "pgtypes.hpp"

class IOPGraph {
public:
    virtual ~IOPGraph() = default;
    virtual void onNodeCreated(type_id id) = 0;
};