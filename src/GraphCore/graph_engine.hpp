#pragma once

#include "graph_types.hpp"
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/LayoutModule.h>
#include <ogdf/basic/geometry.h>

struct GraphState {
    std::vector<GraphNode> nodes;
    std::vector<GraphEdge> edges;
};


enum class GraphAlg {
    BalloonLayout,
    CircularLayout,
    ComponentSplitterLayout,
    DavidsonHarelLayout,
    DominanceLayout,
    FMMMLayout,
    FPPLayout,
    FastMultipoleEmbedder,
    FastMultipoleMultilevelEmbedder,
    GEMLayout,
    MMMExampleFastLayout,
    MMMExampleNiceLayout,
    MMMExampleNoTwistLayout,
    MixedForceLayout,
    MixedModelLayout,
    ModularMultilevelMixer,
    MultilevelLayout,
    PlanarDrawLayout,
    PlanarStraightLayout,
    PlanarizationGridLayout,
    PlanarizationLayout,
    PreprocessorLayout,
    RadialTreeLayout,
    ScalingLayout,
    SchnyderLayout,
    SpringEmbedderFR,
    SpringEmbedderFRExact,
    SpringEmbedderKK,
    StressMinimization,
    SugiyamaLayout,
    TreeLayout,
    TutteLayout,
    UpwardPlanarizationLayout,
    VisibilityLayout
};

class GraphEngine {

public:
    explicit GraphEngine(GraphAlg alg = GraphAlg::FMMMLayout);
    void setAlgorithm(GraphAlg algo);

    //modify the nodes and edges directly
    void calculateLayout(std::vector<std::reference_wrapper<GraphNode>> &nodes,
                         std::vector<std::reference_wrapper<GraphEdge>> &edges);


private:
    static std::string key(uint32_t from, uint32_t to);

    void assignFermatSpiralPositions(
        std::vector<std::reference_wrapper<GraphNode>> &nodes);

private:
    ogdf::Graph m_graph;
    ogdf::GraphAttributes m_attributes;
    std::unique_ptr<ogdf::LayoutModule> m_layout;

    std::unordered_map<uint32_t, ogdf::node> m_idToNode;
    std::unordered_map<std::string, ogdf::edge> m_idToEdge;

    GraphAlg m_algorithm;
};
