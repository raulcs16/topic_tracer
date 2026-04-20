
if(APPLE)
set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/resources/tt.icns")

set_source_files_properties(${app_icon_macos} PROPERTIES
           MACOSX_PACKAGE_LOCATION "Resources")
qt_add_executable(${PROJECT_NAME}Ex
    MACOSX_BUNDLE
    main.cpp
    ${app_icon_macos}
)
set_target_properties(${PROJECT_NAME}Ex PROPERTIES
    MACOSX_BUNDLE_ICON_FILE "tt.icns"
)
else()
qt_add_executable(${PROJECT_NAME}Ex
    main.cpp
)
endif()


qt_add_qml_module(${PROJECT_NAME}Ex
    URI ${PROJECT_NAME}
    VERSION 1.0
    QML_FILES
        Main.qml
    RESOURCES
        "resources/tt.iconset/icon_128x128.png"
    IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY}
)

target_link_libraries(${PROJECT_NAME}Ex PRIVATE
    Qt6::Core
    Qt6::Quick
    GraphQt
    GraphQtplugin
)

set_target_properties(${PROJECT_NAME}Ex PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}"
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.topic_tracer
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}Ex
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY bin/ 
        DESTINATION utilities
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE
)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    
    qt_generate_deploy_qml_app_script(
        TARGET ${PROJECT_NAME}Ex
        OUTPUT_SCRIPT deploy_script
        MACOS_BUNDLE_POST_BUILD
        NO_UNSUPPORTED_PLATFORM_ERROR
        DEPLOY_USER_QML_MODULES_ON_UNSUPPORTED_PLATFORM
    )

    install(SCRIPT ${deploy_script})
    
endif()