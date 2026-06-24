set_source_files_properties(
    GraphConfig.qml
    GraphStore.qml
     PROPERTIES
        QT_QML_SINGLETON_TYPE true
)
qt_add_library(GraphUI SHARED)
qt_add_qml_module(GraphUI
    URI "Graph"
    VERSION 1.0
    DEPENDENCIES
        Views
    QML_FILES
        imports/GraphView.qml
        imports/NodeDelegate.qml
        imports/LabelListView.qml
        imports/LabelDelegate.qml

        GraphApp.qml
        GraphConfig.qml
        GraphController.qml
        GraphStore.qml

    RESOURCES
         GraphEngine.js
    IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY}
)

target_link_libraries(GraphUI PRIVATE
    Qt6::Quick
    ViewsUI
)
target_include_directories(GraphUI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)