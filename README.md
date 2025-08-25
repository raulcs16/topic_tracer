# Topic Tracer

Topic Tracer builds a knowledge graph where topics are represented as nodes and connected with well-defined relationships. Users can traverse this graph to explore dependencies, alternatives, and conceptual hierarchies.

## Graph Features

**Relationship Rules**:

* Concrete ↔ Concrete: depends-on, alternative-to, related-to
* Concrete ↔ Conceptual: example
* Conceptual ↔ Conceptual: composed-of
**Graph Traversal**:

* Shortest Path: find the most direct sequence of topics connecting two nodes.
* Priority List: determine the order of prerequisite topics to cover before reaching a target.
* Topological Sort: generate a valid learning sequence between two nodes, respecting dependency edges.

## Progress Tracking

**Heat Map of Experience**:
As Concrete nodes are mapped to projects, a heat map will emerge that reflects the user’s level of exposure and mastery.
This allows users to visualize where their strengths and gaps are in real time.
**Conceptual Node Completion**:
When all concrete examples of a conceptual topic are marked as covered, the conceptual node is automatically marked as covered.
The same applies recursively to higher-level conceptual topics composed of subtopics.

## Outcome

The result is a personalized knowledge graph where users can:
Plan learning by traversing dependencies.
Track mastery through visual heat maps.
Progress from concrete examples up to broad conceptual understanding.
