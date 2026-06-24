#include "sugiyama_strategy.hpp"
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalRanking.h>

SugiyamaStrategy::SugiyamaStrategy() {

    auto *sl = new ogdf::SugiyamaLayout;
    sl->setRanking(new ogdf::OptimalRanking);
    sl->setCrossMin(new ogdf::MedianHeuristic);

    auto *ohl = new ogdf::OptimalHierarchyLayout;
    ohl->layerDistance(40.0);
    ohl->nodeDistance(75.0);
    ohl->weightBalancing(0.7);
    sl->setLayout(ohl);

    m_layout = sl;
}