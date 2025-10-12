#pragma once

#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/LayoutModule.h>
#include <ogdf/basic/geometry.h>


struct GraphNode {
    uint32_t id;
    double x;
    double y;
};
struct GraphEdge {
    uint32_t from;
    uint32_t to;
    double source_x;
    double source_y;
    double target_x;
    double target_y;
    std::vector<ogdf::DPoint> bends;
};


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

    void setState(const GraphState &state);
    void calculateLayout();

    const std::vector<GraphNode> &nodes() const;
    const std::vector<GraphEdge> &edges() const;

private:
    void invalidate();
    static std::string key(uint32_t from, uint32_t to);

private:
    ogdf::Graph m_graph;
    ogdf::GraphAttributes m_attributes;
    std::unique_ptr<ogdf::LayoutModule> m_layout;

    std::unordered_map<uint32_t, ogdf::node> m_idToNode;
    std::unordered_map<std::string, ogdf::edge> m_idToEdge;


    GraphState m_state;
    GraphAlg m_algorithm;
    bool m_latest;
};
