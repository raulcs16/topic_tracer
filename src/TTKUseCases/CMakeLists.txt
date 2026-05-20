add_library(TTKUseCases STATIC
    terminal_out.hpp
    get_auto_suggestion.hpp 
    handle_input.hpp
    terminal_controller.hpp
)
target_link_libraries(TTKUseCases PUBLIC TerminalToolKit)
target_include_directories(TerminalToolKit
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)
