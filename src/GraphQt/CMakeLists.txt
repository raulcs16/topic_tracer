qt_add_library(GraphQt STATIC)

set_target_properties(GraphQt PROPERTIES AUTOMOC ON)

target_link_libraries(GraphQt
    PRIVATE Qt6::Core
    PRIVATE Qt6::Quick
    PRIVATE OGDF
    PUBLIC GraphCore
)

qt_add_qml_module(GraphQt
    URI Graph
    VERSION 1.0
    SOURCES

        graph_controller.cpp
        graph_controller.hpp
        
        node_list_model.hpp
        node_list_model.cpp

        edge_list_model.hpp
        edge_list_model.cpp

    QML_FILES
        EdgeSpline.qml
        GraphView.qml
    IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY}
)

target_include_directories(GraphQt PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

