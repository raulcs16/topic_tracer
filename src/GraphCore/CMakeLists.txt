add_library(GraphCore STATIC
    graph_types.hpp
    graph.cpp
    graph.hpp
    edge_semantic.hpp
    path_analyzer.hpp
    path_analyzer.cpp
)
target_link_libraries(GraphCore PUBLIC Utils)
target_include_directories(GraphCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)