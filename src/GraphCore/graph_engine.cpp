#include "graph_engine.hpp"

#include <ogdf/basic/PreprocessorLayout.h>
#include <ogdf/energybased/DavidsonHarelLayout.h>
#include <ogdf/energybased/FMMMLayout.h>
#include <ogdf/energybased/FastMultipoleEmbedder.h>
#include <ogdf/energybased/GEMLayout.h>
#include <ogdf/energybased/MultilevelLayout.h>
// #include <ogdf/energybased/SpringEmbedderFR.h>
#include <ogdf/energybased/SpringEmbedderFRExact.h>
#include <ogdf/energybased/SpringEmbedderKK.h>
#include <ogdf/energybased/StressMinimization.h>
#include <ogdf/energybased/TutteLayout.h>
// #include <ogdf/energybased/multilevel_mixer/MMMExampleFastLayout.h>
// #include <ogdf/energybased/multilevel_mixer/MMMExampleNiceLayout.h>
// #include <ogdf/energybased/multilevel_mixer/MMMExampleNoTwistLayout.h>
// #include <ogdf/energybased/multilevel_mixer/MixedForceLayout.h>
#include <ogdf/energybased/multilevel_mixer/ModularMultilevelMixer.h>
#include <ogdf/energybased/multilevel_mixer/ScalingLayout.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/misclayout/BalloonLayout.h>
#include <ogdf/misclayout/CircularLayout.h>
#include <ogdf/packing/ComponentSplitterLayout.h>
#include <ogdf/planarity/PlanarizationGridLayout.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarlayout/FPPLayout.h>
#include <ogdf/planarlayout/MixedModelLayout.h>
#include <ogdf/planarlayout/PlanarDrawLayout.h>
#include <ogdf/planarlayout/PlanarStraightLayout.h>
#include <ogdf/planarlayout/SchnyderLayout.h>
#include <ogdf/tree/RadialTreeLayout.h>
#include <ogdf/tree/TreeLayout.h>
#include <ogdf/upward/DominanceLayout.h>
#include <ogdf/upward/UpwardPlanarizationLayout.h>
#include <ogdf/upward/VisibilityLayout.h>


GraphEngine::GraphEngine(GraphAlg alg)
    : m_graph(), m_attributes(m_graph,
                              ogdf::GraphAttributes::nodeGraphics |
                                  ogdf::GraphAttributes::edgeGraphics |
                                  ogdf::GraphAttributes::nodeLabel |
                                  ogdf::GraphAttributes::edgeStyle),
      m_algorithm(alg), m_layout(new ogdf::FMMMLayout), m_latest(false) {
    setAlgorithm(GraphAlg::FMMMLayout);
}

void GraphEngine::setAlgorithm(GraphAlg algo) {

    ogdf::LayoutModule *layout = 0;
    switch (algo) {
    case GraphAlg::BalloonLayout: layout = new ogdf::BalloonLayout(); break;
    case GraphAlg::CircularLayout: layout = new ogdf::CircularLayout(); break;
    case GraphAlg::ComponentSplitterLayout:
        layout = new ogdf::ComponentSplitterLayout;
        break;
    case GraphAlg::DavidsonHarelLayout: layout = new ogdf::DavidsonHarelLayout; break;
    case GraphAlg::DominanceLayout: layout = new ogdf::DominanceLayout; break;
    case GraphAlg::FMMMLayout: {
        auto *fmmm = new ogdf::FMMMLayout();
        fmmm->unitEdgeLength(100.0);
        fmmm->repForcesStrength(10);
        layout = fmmm;
        break;
    }
    case GraphAlg::FPPLayout: layout = new ogdf::FPPLayout; break;
    case GraphAlg::FastMultipoleEmbedder: layout = new ogdf::FastMultipoleEmbedder; break;
    case GraphAlg::FastMultipoleMultilevelEmbedder:
        layout = new ogdf::FastMultipoleMultilevelEmbedder;
        break;
    case GraphAlg::GEMLayout: layout = new ogdf::GEMLayout; break;
    case GraphAlg::MixedModelLayout: layout = new ogdf::MixedModelLayout; break;
    case GraphAlg::ModularMultilevelMixer:
        layout = new ogdf::ModularMultilevelMixer;
        break;
    case GraphAlg::MultilevelLayout: layout = new ogdf::MultilevelLayout; break;
    case GraphAlg::PlanarDrawLayout: layout = new ogdf::PlanarDrawLayout; break;
    case GraphAlg::PlanarStraightLayout: layout = new ogdf::PlanarStraightLayout; break;
    case GraphAlg::PlanarizationGridLayout:
        layout = new ogdf::PlanarizationGridLayout;
        break;
    case GraphAlg::PlanarizationLayout: layout = new ogdf::PlanarizationLayout; break;
    case GraphAlg::PreprocessorLayout: layout = new ogdf::PreprocessorLayout; break;
    case GraphAlg::RadialTreeLayout: layout = new ogdf::RadialTreeLayout; break;
    case GraphAlg::ScalingLayout: layout = new ogdf::ScalingLayout; break;
    case GraphAlg::SchnyderLayout: layout = new ogdf::SchnyderLayout; break;
    case GraphAlg::SpringEmbedderFRExact: layout = new ogdf::SpringEmbedderFRExact; break;
    case GraphAlg::SpringEmbedderKK: layout = new ogdf::SpringEmbedderKK; break;
    case GraphAlg::StressMinimization: layout = new ogdf::StressMinimization; break;
    case GraphAlg::SugiyamaLayout: layout = new ogdf::SugiyamaLayout; break;
    case GraphAlg::TreeLayout: layout = new ogdf::TreeLayout; break;
    case GraphAlg::TutteLayout: layout = new ogdf::TutteLayout; break;
    case GraphAlg::UpwardPlanarizationLayout:
        layout = new ogdf::UpwardPlanarizationLayout;
        break;
    case GraphAlg::VisibilityLayout: layout = new ogdf::VisibilityLayout; break;
    default: break;
    }
    if (layout) {
        m_layout.reset(layout);
        m_algorithm = algo;
        invalidate();
    }
}
void GraphEngine::invalidate() { m_latest = false; }

void GraphEngine::calculateLayout() {
    if (m_latest)
        return;

    if (m_state.edges.size() == 0) {
        assignFermatSpiralPositions(m_state.nodes);
        m_latest = true;
        return;
    }
    try {

        m_layout->call(m_attributes);
        m_attributes.addNodeCenter2Bends();
        for (auto &node : m_state.nodes) {
            auto it = m_idToNode.find(node.id);
            if (it == m_idToNode.end())
                continue;
            node.x = m_attributes.x(it->second);
            node.y = m_attributes.y(it->second);
        }
        for (auto &edge : m_state.edges) {
            auto src = m_idToNode.find(edge.from);
            auto target = m_idToNode.find(edge.to);
            if (src == m_idToNode.end() || target == m_idToNode.end())
                continue;

            edge.source_x = m_attributes.x(src->second);
            edge.source_y = m_attributes.y(src->second);
            edge.target_x = m_attributes.x(target->second);
            edge.target_y = m_attributes.y(target->second);
            auto e = m_idToEdge.find(key(edge.from, edge.to));
            if (e == m_idToEdge.end())
                continue;
            edge.bends.clear();
            for (ogdf::DPoint &p : m_attributes.bends(e->second)) {
                edge.bends.push_back(p);
            }
        }


        m_latest = true;
    } catch (ogdf::AlgorithmFailureException &e) {
        std::cerr << "Algorithmn Failure: " << (int)(e.exceptionCode()) << "\n";
    } catch (...) {
        std::cerr << "unknown error\n";
    }
}

void GraphEngine::setState(const GraphState &state) {
    m_graph.clear();
    m_idToNode.clear();
    m_state = state;
    invalidate();

    for (auto node : state.nodes) {
        auto n = m_graph.newNode();
        m_idToNode[node.id] = n;
    }
    for (auto edge : state.edges) {
        auto e = m_graph.newEdge(m_idToNode[edge.from], m_idToNode[edge.to]);
        m_idToEdge[key(edge.from, edge.to)] = e;
    }
}
const std::vector<GraphNode> &GraphEngine::nodes() const { return m_state.nodes; }
const std::vector<GraphEdge> &GraphEngine::edges() const { return m_state.edges; }

std::string GraphEngine::key(uint32_t from, uint32_t to) {
    return std::to_string(from) + "->" + std::to_string(to);
}
void GraphEngine::assignFermatSpiralPositions(std::vector<GraphNode> &nodes) {
    const double golden = M_PI * (3 - std::sqrt(5));
    const double scale = 50.0;
    for (int i = 0; i < nodes.size(); i++) {
        double r = scale * std::sqrt(i + 1);
        double theta = i * golden;
        nodes[i].x = r * std::cos(theta);
        nodes[i].y = r * std::sin(theta);
    }
}