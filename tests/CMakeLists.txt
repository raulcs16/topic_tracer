include(Catch)


add_executable(TopicGraphUnderTest test_topic_graph.cpp)
target_link_libraries(TopicGraphUnderTest PUBLIC TopicGraphCore)
target_link_libraries(TopicGraphUnderTest PUBLIC Catch2::Catch2WithMain)

add_executable(LayoutEngineUnderTest test_layout_engine.cpp)
target_link_libraries(LayoutEngineUnderTest PUBLIC LayoutEngine)
target_link_libraries(LayoutEngineUnderTest PUBLIC Catch2::Catch2WithMain)


catch_discover_tests(TopicGraphUnderTest)
catch_discover_tests(LayoutEngineUnderTest)

if(COVERAGE)
    set(COVERAGE_MAIN coverage)
    set(COVERAGE_EXCLUDES
        "${PROJECT_SOURCE_DIR}/app/*"
        "${PROJECT_SOURCE_DIR}/cmake/*"
        "${PROJECT_SOURCE_DIR}/docs/*"
        "${PROJECT_SOURCE_DIR}/external/*"
        "${PROJECT_SOURCE_DIR}/tests/*"
        "${PROJECT_SOURCE_DIR}/build/*"
        "/usr/include/*"
    )
    set(COVERAGE_EXTRA_FLAGS)
    set(COVERAGE_DEPENDENCIES TopicGraphUnderTest LayoutEngineUnderTest)

    setup_target_for_coverage_gcovr_html(
        NAME
            ${COVERAGE_MAIN}
        EXECUTABLE
            ctest
        DEPENDENCIES
            ${COVERAGE_DEPENDENCIES}
        BASE_DIRECTORY
            ${CMAKE_SOURCE_DIR}/src
        EXCLUDE
            ${COVERAGE_EXCLUDES}
             
    )
endif()
        