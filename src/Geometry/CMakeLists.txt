add_library(Geometry STATIC
   geometry.hpp 
)
target_include_directories(Geometry
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)


qt_add_library(GeometryQt STATIC)
set_target_properties(GeometryQt PROPERTIES AUTOMOC ON)
target_link_libraries(GeometryQt
    PRIVATE Qt6::Core
    PUBLIC Geometry
)

qt_add_qml_module(GeometryQt
    URI GeometryQt
    VERSION 1.0
    DEPENDENCIES 
    SOURCES
     geometryqt.hpp
    QML_FILES
    IMPORT_PATH 
        ${QT_QML_OUTPUT_DIRECTORY}
)

target_include_directories(GeometryQt PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
        "${CMAKE_BINARY_DIR}/configured_files/include"
)
