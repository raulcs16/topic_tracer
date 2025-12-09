add_library(TopicGraphCore STATIC
    topic_graph_types.hpp

    itopic_store.hpp
    itopic_store_listener.hpp
    iedge_store.hpp
    iedge_store_listener.hpp

    topic_store.hpp
    topic_store.cpp

    edge_store.hpp
    edge_store.cpp
    
    topic_graph.cpp
    topic_graph.hpp
    edge_semantic.hpp
    path_analyzer.hpp
    path_analyzer.cpp
)
target_link_libraries(TopicGraphCore PUBLIC Utils)
target_include_directories(TopicGraphCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)