qt_add_library(StylesLib STATIC)

set(SINGLETON_SRC
    Colors.qml
)

set_source_files_properties(${SINGLETON_SRC} PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(StylesLib
    URI Styles
    VERSION 1.0
    QML_FILES
        Colors.qml
 )

set_target_properties(StylesLib PROPERTIES AUTOMOC ON)
target_link_libraries(StylesLib PRIVATE Qt6::Quick)
