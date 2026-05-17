add_library(TerminalToolKit STATIC

    default_keys.hpp

    token.hpp

    tokenizer.hpp
    tokenizer.cpp

    command_signature.hpp
    command_signature.cpp

    icommand.hpp

    iprovider.hpp
    suggestion_engine.hpp
    suggestion_engine.cpp

    cli_service.hpp
    cli_service.cpp

)
target_link_libraries(TerminalToolKit)
target_include_directories(TerminalToolKit
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)
