#pragma once

#include "layout_types.hpp"
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/LayoutModule.h>
#include <ogdf/basic/geometry.h>


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

class LayoutEngine {

public:
    explicit LayoutEngine(GraphAlg alg = GraphAlg::FMMMLayout);
    void setAlgorithm(GraphAlg algo);

    //incremental ops
    void addNode(uint32_t id);
    void removeNode(uint32_t id);
    void addEdge(uint32_t from, uint32_t to);
    void removeEdge(const std::string &key);

    void clear();

    //modify the nodes and edges directly
    void calculateLayout();

    const std::vector<GraphNode> &nodes() const { return m_nodes; }
    const std::vector<GraphEdge> &edges() const { return m_edges; }

private:
    static std::string key(uint32_t from, uint32_t to);

    void assignFermatSpiralPositions();

private:
    ogdf::Graph m_graph;
    ogdf::GraphAttributes m_attributes;
    std::unique_ptr<ogdf::LayoutModule> m_layout;

    std::unordered_map<uint32_t, ogdf::node> m_idToNode;
    std::unordered_map<std::string, ogdf::edge> m_idToEdge;

    std::vector<GraphNode> m_nodes;
    std::vector<GraphEdge> m_edges;

    GraphAlg m_algorithm;
};
