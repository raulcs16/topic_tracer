add_library(Geometry STATIC
   geometry.hpp 
)
target_include_directories(Geometry
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)