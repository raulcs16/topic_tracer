set_source_files_properties(
    TerminalConfig.qml
    TerminalStore.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE true
)
qt_add_library(TerminalUI SHARED)
qt_add_qml_module(TerminalUI
    URI "Terminal"
    VERSION 1.0
    DEPENDENCIES
    QML_FILES
        TerminalApp.qml
        TerminalConfig.qml
        TerminalController.qml
        TerminalStore.qml
        imports/TerminalView.qml
        imports/TerminalInput.qml
        imports/EntryDelegate.qml
    RESOURCES
        TerminalEngine.js
    IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY}
)
target_link_libraries(TerminalUI PRIVATE
    Qt6::Quick
)
target_include_directories(TerminalUI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)