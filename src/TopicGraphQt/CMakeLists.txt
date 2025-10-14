qt_add_library(TopicGraphQt STATIC)


set_target_properties(TopicGraphQt PROPERTIES AUTOMOC ON)

target_link_libraries(TopicGraphQt
    PRIVATE Qt6::Core
    PRIVATE Qt6::Quick
    PUBLIC TopicGraphCore
    PUBLIC TopicListQt
    PUBLIC GraphQt
)

qt_add_qml_module(TopicGraphQt
    URI TopicGraph
    VERSION 1.0
    SOURCES
        topic_graph_controller.hpp
        topic_graph_controller.cpp
)

target_include_directories(TopicGraphQt PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)
