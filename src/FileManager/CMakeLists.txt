qt_add_library(FileManagerQt STATIC)

set_target_properties(FileManagerQt PROPERTIES AUTOMOC ON)
target_link_libraries(FileManagerQt
    PRIVATE Qt6::Core
    PRIVATE Qt6::Quick
)

qt_add_qml_module(FileManagerQt
    URI FileManagerQt
    VERSION 1.0
    SOURCES
        ifile_manager.hpp
        # file_manager_qt.hpp
        # file_manager_qt.cpp
        file_manager_dev.hpp
        file_manager_dev.cpp
    QML_FILES
    IMPORT_PATH 
    ${QT_QML_OUTPUT_DIRECTORY}
)

target_include_directories(FileManagerQt PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
        "${CMAKE_BINARY_DIR}/configured_files/include"
)
