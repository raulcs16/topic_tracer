include(Catch)

set(TEST_MAIN "unit_tests")
set(TEST_SOURCES
#test files
graph_core_tests.cpp
evidence_tests.cpp
layout_engine_tests.cpp
)
set(TEST_INCLUDES "./")

add_executable(${TEST_MAIN} ${TEST_SOURCES})
target_include_directories(${TEST_MAIN} PUBLIC ${TEST_INCLUDES})
target_link_libraries(${TEST_MAIN} PUBLIC 
#libraries
LayoutEngine
TopicGraphCore
Evidence

Catch2::Catch2WithMain)

catch_discover_tests(${TEST_MAIN})
