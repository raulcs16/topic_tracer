#include "fmmm_strategy.hpp"


FMMMStrategy::FMMMStrategy() {
    auto *fmmm = new ogdf::FMMMLayout();
    fmmm->unitEdgeLength(100.0);
    fmmm->repForcesStrength(10);
    m_layout = fmmm;
}