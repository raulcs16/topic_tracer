add_library(LayoutEngine STATIC
    #ds helpers
    disjoint_set.hpp
    disjoint_set.cpp
    #interface
    icomponent_layout.hpp
    pool_component.hpp
    pool_component.cpp
    ogdf_component.hpp
    ogdf_component.cpp
    #geometric data
    layout_types.hpp
    layout_engine.cpp
    layout_engine.hpp
    layout_strategy.hpp
    ogdf_strategy.hpp
    ogdf_strategy.cpp
    #ogdf strats
    sugiyama_strategy.hpp
    sugiyama_strategy.cpp
    fmmm_strategy.hpp
    fmmm_strategy.cpp
    orthogonal_strategy.hpp
    orthogonal_strategy.cpp
    # procedural strats
    fermatspiral_strategy.hpp
    fermatspiral_strategy.cpp
)

target_link_libraries(LayoutEngine
    PUBLIC OGDF
    PUBLIC Utils
)
target_include_directories(LayoutEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR} 
)