add_library(GraphCore STATIC
    graph_engine.cpp
    graph_engine.hpp

)
target_link_libraries(GraphCore
    PUBLIC OGDF
)
target_include_directories(GraphCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR} 
)