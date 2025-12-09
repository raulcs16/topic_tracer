qt_add_library(Canvas SHARED)


qt_add_qml_module(Canvas 
    URI "Canvas"
    VERSION 1.0
    QML_FILES
        CanvasView.qml
    IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY}
)

target_link_libraries(Canvas PRIVATE Qt6::Quick)