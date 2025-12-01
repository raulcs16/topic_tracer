add_library(Persistence STATIC
    topic_graph_repo.hpp
    topic_graph_repo.cpp
)
target_link_libraries(Persistence
    PRIVATE Qt6::Core
    PUBLIC TopicGraphCore
)
target_include_directories(Persistence
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)