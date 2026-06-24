cmake_minimum_required(VERSION 3.16)

project(TopicTracer VERSION 0.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

option(APPS "Enable App Build" OFF)
option(QT "Enable QT Framework" OFF)
option(MAIN "Enable Main Build" OFF)

option(COVERAGE "Enable Code Coverage" OFF)
option(TESTING "Enable Unit Testing" OFF)

include(FetchContent)

if(TESTING)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.3
    )
    FetchContent_MakeAvailable(Catch2)
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

    if(COVERAGE)
        include(CodeCoverage)
        append_coverage_compiler_flags()
    endif()

    include(CTest)
    enable_testing()

endif()

add_subdirectory(configured)

##CORE
add_subdirectory(src/core/cli)
# add_subdirectory(src/core/evidence)
# add_subdirectory(src/core/filemanager)
# add_subdirectory(src/core/geometry)
# add_subdirectory(src/core/graph)
# add_subdirectory(src/core/layoutengine)
# add_subdirectory(src/core/property-graph)
# add_subdirectory(src/core/utils)


if(QT)

    set(QT_QML_GENERATE_QMLLS_INI ON)
    find_package(Qt6 REQUIRED COMPONENTS Quick)
    find_package(Qt6 REQUIRED COMPONENTS Core)
    qt_standard_project_setup(REQUIRES 6.5)

    set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml)
    set(QML_IMPORT_PATH "${QT_QML_OUTPUT_DIRECTORY}" CACHE STRING "QML import paths")

    # add_subdirectory(src/qt-plugins/cli)
    # add_subdirectory(src/qt-plugins/filemanager)
    # add_subdirectory(src/qt-plugins/geometry)
    # add_subdirectory(src/qt-plugins/graph)

    if(APPS)
        add_subdirectory(apps/qt/imports/views)
        add_subdirectory(apps/qt/knowledge-graph)
        add_subdirectory(apps/qt/terminal-simulator)
        if(MAIN)
            # add_subdirectory(apps/qt/main)
        endif()
    endif()

endif()


configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/metadata.mk.in"
    "${CMAKE_CURRENT_BINARY_DIR}/metadata.mk"
    @ONLY
)