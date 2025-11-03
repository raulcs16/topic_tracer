#pragma once

#include "ogdf_strategy.hpp"
#include <ogdf/layered/SugiyamaLayout.h>

class SugiyamaStrategy : public OGDFStrat {
public:
    explicit SugiyamaStrategy(OGDFContext &ctx);
};