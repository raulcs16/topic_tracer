#include "orthogonal_strategy.hpp"
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
#include <ogdf/planarity/PlanarSubgraphFast.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/SubgraphPlanarizer.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>

OrthogonalStrategy::OrthogonalStrategy(OGDFContext &context) : OGDFStrat{context} {
    ogdf::PlanarizationLayout *pl = new ogdf::PlanarizationLayout;

    ogdf::SubgraphPlanarizer *crossMin = new ogdf::SubgraphPlanarizer;
    ogdf::PlanarSubgraphFast<int> *ps = new ogdf::PlanarSubgraphFast<int>;
    ps->runs(100);
    ogdf::VariableEmbeddingInserter *ves = new ogdf::VariableEmbeddingInserter;
    ves->removeReinsert(ogdf::RemoveReinsertType::All);

    crossMin->setSubgraph(ps);
    crossMin->setInserter(ves);
    pl->setCrossMin(crossMin);

    ogdf::EmbedderMinDepthMaxFaceLayers *emb = new ogdf::EmbedderMinDepthMaxFaceLayers;
    pl->setEmbedder(emb);

    ogdf::OrthoLayout *ol = new ogdf::OrthoLayout;
    ol->separation(50.0);
    ol->cOverhang(0.4);
    pl->setPlanarLayouter(ol);

    m_layout = pl;
}
