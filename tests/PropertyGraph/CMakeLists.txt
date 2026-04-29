
add_executable(EdgeRegUT test_edge_registry.cpp)
target_link_libraries(EdgeRegUT PUBLIC PropertyGraph Catch2::Catch2WithMain Qt6::Core)
catch_discover_tests(EdgeRegUT PROPERTIES LABELS "PGraph")

add_executable(NodeRegUT test_node_registry.cpp)
target_link_libraries(NodeRegUT PUBLIC PropertyGraph Catch2::Catch2WithMain Qt6::Core)
catch_discover_tests(NodeRegUT PROPERTIES LABELS "PGraph")


add_executable(PGraphUT test_pgraph.cpp)
target_link_libraries(PGraphUT PUBLIC PropertyGraph Catch2::Catch2WithMain Qt6::Core)
catch_discover_tests(PGraphUT PROPERTIES LABELS "PGraph")