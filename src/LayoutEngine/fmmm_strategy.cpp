#include "fmmm_strategy.hpp"


FMMMStrategy::FMMMStrategy() {
    auto *fmmm = new ogdf::FMMMLayout();
    fmmm->unitEdgeLength(100.0);
    fmmm->repForcesStrength(100.0);
    fmmm->qualityVersusSpeed(ogdf::FMMMOptions::QualityVsSpeed::GorgeousAndEfficient);
    m_layout = fmmm;
}