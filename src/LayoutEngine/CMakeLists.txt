add_library(LayoutEngine STATIC
    layout_types.hpp
    layout_engine.cpp
    layout_engine.hpp

)
target_link_libraries(LayoutEngine
    PUBLIC OGDF
)
target_include_directories(LayoutEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR} 
)