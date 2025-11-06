add_library(TopicGraphCore STATIC
    topic_graph_types.hpp
    topic_graph.cpp
    topic_graph.hpp
    path_analyzer.hpp
    path_analyzer.cpp
)
target_include_directories(TopicGraphCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)