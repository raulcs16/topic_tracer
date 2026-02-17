add_library(Persistence STATIC
    graph_repo.hpp
    graph_repo.cpp
)
target_link_libraries(Persistence
    PRIVATE Qt6::Core
    PUBLIC GraphCore
)
target_include_directories(Persistence
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)