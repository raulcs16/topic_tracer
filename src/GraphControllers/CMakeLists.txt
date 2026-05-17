qt_add_library(GraphControllers STATIC)

set_target_properties(GraphControllers PROPERTIES AUTOMOC ON)

target_link_libraries(GraphControllers
    PUBLIC Qt6::Core
    PUBLIC TerminalQt
    PUBLIC TerminalQtplugin
    PUBLIC TerminalToolKit
)

qt_add_qml_module(GraphControllers
    URI GraphControllers
    VERSION 1.0
    DEPENDENCIES 
        TerminalQt
    SOURCES
        terminal_controller.hpp
        terminal_controller.cpp
    IMPORT_PATH 
        ${QT_QML_OUTPUT_DIRECTORY}
)

target_include_directories(GraphControllers PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
        "${CMAKE_BINARY_DIR}/configured_files/include"
)

# add_library(GraphControllers STATIC
#     terminal_controller.hpp
#     terminal_controller.cpp
# )
# set_target_properties(GraphControllers PROPERTIES AUTOMOC ON)
# target_link_libraries(GraphControllers
#     PUBLIC Qt6::Core
#     PUBLIC TerminalQt
#     PUBLIC TerminalQtplugin
#     PUBLIC TerminalToolKit
# )
# target_include_directories(GraphControllers
#     PUBLIC
#         ${CMAKE_CURRENT_SOURCE_DIR}  
# )