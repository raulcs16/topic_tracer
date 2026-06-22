add_library(CLI STATIC
    cli.h
    cli.cpp
)
target_link_libraries(CLI)
target_include_directories(CLI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)
