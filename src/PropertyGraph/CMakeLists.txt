add_library(PropertyGraph STATIC
    pgtypes.hpp
    nodes.hpp
    edges.hpp

    # node_registry.hpp
    # node_registry.cpp

    node_type_registry.hpp
    node_type_registry.cpp

    # edge_registry.hpp
    # edge_registry.cpp

    edge_type_registry.hpp
    edge_type_registry.cpp

    pgraph.hpp
    pgraph.cpp
)
target_link_libraries(PropertyGraph
    PUBLIC Qt6::Core
    PUBLIC Geometry
)
target_include_directories(PropertyGraph
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)
