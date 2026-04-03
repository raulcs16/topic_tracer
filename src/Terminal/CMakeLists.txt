qt_add_library(TerminalQt STATIC)

set_target_properties(TerminalQt PROPERTIES AUTOMOC ON)
target_link_libraries(TerminalQt
    PRIVATE Qt6::Core
    PRIVATE Qt6::Quick
)

qt_add_qml_module(TerminalQt
    URI TerminalQt
    VERSION 1.0
    SOURCES
        terminal_list_model.cpp
        terminal_list_model.hpp
    QML_FILES
        EntryDelegate.qml
        TerminalInput.qml
        TerminalView.qml
    IMPORT_PATH 
    ${QT_QML_OUTPUT_DIRECTORY}
)

target_include_directories(TerminalQt PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
        "${CMAKE_BINARY_DIR}/configured_files/include"
)
