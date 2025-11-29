add_library(Evidence STATIC
    evidence.hpp
    evidence.cpp
    heat_score.hpp
    heat_score.cpp
)

target_link_libraries(Evidence PUBLIC TopicGraphCore)

target_include_directories(Evidence
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)