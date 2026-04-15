qt_add_library(VisualState STATIC)


set_target_properties(VisualState PROPERTIES AUTOMOC ON)
target_link_libraries(VisualState PRIVATE Qt6::Quick)

target_link_libraries(VisualState
    PRIVATE Qt6::Core
    PRIVATE Qt6::Quick
    PUBLIC Geometry
)

qt_add_qml_module(VisualState
    URI VisualState
    VERSION 1.0
    DEPENDENCIES 
    SOURCES
        istate_store.hpp
        visual_types.hpp
        visual_states.hpp
    QML_FILES
    IMPORT_PATH 
        ${QT_QML_OUTPUT_DIRECTORY}
)

target_include_directories(VisualState PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
        "${CMAKE_BINARY_DIR}/configured_files/include"
)
