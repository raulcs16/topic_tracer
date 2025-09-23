add_library(TopicGraphCore STATIC
    topic_graph.cpp
    topic_graph.hpp
    edge_type.hpp
)
target_link_libraries(TopicGraphCore PUBLIC TopicCore)
target_include_directories(TopicGraphCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)