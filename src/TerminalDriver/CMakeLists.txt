add_library(TerminalDriver STATIC
    node_command.hpp
)
target_link_libraries(TerminalDriver
    PUBLIC Qt6::Core
    PUBLIC TerminalToolKit
    PUBLIC Interactors
)
target_include_directories(TerminalDriver
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)
