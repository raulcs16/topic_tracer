add_library(CoreLib STATIC 
    topic_graph.cpp
)
target_include_directories(CoreLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
