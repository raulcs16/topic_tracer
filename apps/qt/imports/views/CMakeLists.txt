qt_add_library(ViewsUI SHARED)
qt_add_qml_module(ViewsUI
    URI "Views"
    VERSION 1.0
    DEPENDENCIES
    QML_FILES
        ViewPort.qml
        ViewPortDriver.qml
    RESOURCES
    IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY}
)
target_link_libraries(ViewsUI PRIVATE
    Qt6::Quick
)
target_include_directories(ViewsUI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)