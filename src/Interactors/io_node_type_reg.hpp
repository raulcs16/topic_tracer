#pragma once
#include "pgtypes.hpp"

class IONodeTypeReg {
public:
    virtual ~IONodeTypeReg() = default;
    virtual void onTypeCreated(type_id id) = 0;
};