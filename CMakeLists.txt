cmake_minimum_required(VERSION 3.16)

project(TopicTracer VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

option(TESTING "Enable Unit Testing" OFF)
option(COVERAGE "Enable Code Coverage" OFF)

if(TESTING AND COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup(REQUIRES 6.5)

set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml)
set(QML_IMPORT_PATH "${QT_QML_OUTPUT_DIRECTORY}" CACHE STRING "QML import paths")

# FetchContent
include(FetchContent)


if(TESTING)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.3
    )
    FetchContent_MakeAvailable(Catch2)
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
endif()

FetchContent_Declare(
    ogdf
    GIT_REPOSITORY https://github.com/ogdf/ogdf.git
    GIT_TAG  elderberry-202309
)
FetchContent_MakeAvailable(ogdf)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(nlohmann_json)


add_subdirectory(configured)
add_subdirectory(src)
if (NOT TESTING)
    add_subdirectory(app)
endif( )

if(TESTING)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()

