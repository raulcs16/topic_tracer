add_library(TopicCore STATIC
    topic.cpp
    topic.hpp
    topic_type.hpp
)

target_include_directories(TopicCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}    # so other targets can #include "topic.hpp"
)