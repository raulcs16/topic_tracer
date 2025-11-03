#pragma once

#include "ogdf_strategy.hpp"
#include <ogdf/energybased/FMMMLayout.h>


class FMMMStrategy : public OGDFStrat {
public:
    explicit FMMMStrategy(OGDFContext &ctx);
};