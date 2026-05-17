
qt_add_executable(TerminalEx
    main.cpp
)

qt_add_qml_module(TerminalEx
    URI Terminal
    VERSION 1.0
    DEPENDENCIES 
        TerminalQt
        Styles
    QML_FILES
        Terminal.qml
    RESOURCES
    IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY}
)

target_link_libraries(TerminalEx
    PUBLIC Qt6::Core
    PUBLIC Qt6::Quick
    PUBLIC GraphControllers
    PUBLIC TerminalQt
    PUBLIC TerminalQtplugin
)

set_target_properties(TerminalEx PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.topic_tracer.terminal
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TerminalEx
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
