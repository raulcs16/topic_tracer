
qt_add_library(TopicLib STATIC)

set_target_properties(TopicLib PROPERTIES AUTOMOC ON)
target_link_libraries(TopicLib PRIVATE 
    CoreLib
    Qt6::Quick
)


qt_add_qml_module(TopicLib
    URI Topic
    VERSION 1.0
    SOURCES
        topic_qt.hpp
        topic_qt.cpp
        topic_graph_qt.hpp
        topic_graph_qt.cpp
 )
