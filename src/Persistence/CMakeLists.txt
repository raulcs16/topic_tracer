add_library(Persistence STATIC
    graph_repo.hpp
    graph_repo.cpp
)
target_link_libraries(Persistence
    PUBLIC Qt6::Core
    PUBLIC GraphCore
    PUBLIC FileManager
)
target_include_directories(Persistence
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)