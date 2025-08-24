cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME TopicTracer)
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup(REQUIRES 6.5)

set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml)
set(QML_IMPORT_PATH "${QT_QML_OUTPUT_DIRECTORY}" CACHE STRING "QML import paths")
set(TESTING OFF)

# FetchContent
include(FetchContent)

if(TESTING)
include(CTest)
enable_testing()
set(CATCH2_SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/catch2 CACHE PATH "")
FetchContent_Declare(
    Catch2
    SOURCE_DIR ${CATCH2_SOURCE_DIR}
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.5.3
)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
endif()


add_subdirectory(src)
add_subdirectory(app)

if(TESTING)
add_subdirectory(tests)
endif()

