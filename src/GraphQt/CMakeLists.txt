qt_add_library(GraphQt STATIC)

set_target_properties(GraphQt PROPERTIES AUTOMOC ON)

target_link_libraries(GraphQt
    PRIVATE Qt6::Core
    PRIVATE Qt6::Quick
    PRIVATE OGDF
)

qt_add_qml_module(GraphQt
    URI Graph
    VERSION 1.0
    SOURCES
        #https://github.com/schulzch/qml-ogdf/tree/master/ogdfplugin
        graph.hpp
        graph.cpp
        
        graph_layout.hpp
        graph_layout.cpp

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

