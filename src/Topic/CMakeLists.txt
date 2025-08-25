
add_library(TopicLib INTERFACE)
target_sources(TopicLib INTERFACE topic.hpp)
target_include_directories(TopicLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})