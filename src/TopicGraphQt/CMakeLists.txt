qt_add_library(TopicGraphQt STATIC)


set_target_properties(TopicGraphQt PROPERTIES AUTOMOC ON)
target_link_libraries(TopicGraphQt PRIVATE Qt6::Quick)

target_link_libraries(TopicGraphQt
    PRIVATE Qt6::Core
    PRIVATE Qt6::Quick
    PUBLIC TopicGraphCore
    PUBLIC LayoutEngine
    PUBLIC OGDF
)

qt_add_qml_module(TopicGraphQt
    URI TopicGraph
    VERSION 1.0
    SOURCES
        topic_graph_controller.hpp
        topic_graph_controller.cpp

        topic_list_model.hpp
        topic_list_model.cpp

        edge_list_model.hpp
        edge_list_model.cpp

        node_list_model.hpp
        node_list_model.cpp
        
        ui_states.hpp
        ui_states.cpp

    QML_FILES
        TopicListView.qml
        GraphView.qml
        EdgeSpline.qml

    IMPORT_PATH 
        ${QT_QML_OUTPUT_DIRECTORY}
)

target_include_directories(TopicGraphQt PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)
