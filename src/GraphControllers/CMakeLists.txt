add_library(GraphControllers STATIC
    cli_term_controller.hpp
    cli_term_controller.cpp
)
target_link_libraries(GraphControllers
    PUBLIC Qt6::Core
    PUBLIC TerminalQt
    PUBLIC TerminalQtplugin
    PUBLIC TerminalToolKit
)
target_include_directories(GraphControllers
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)