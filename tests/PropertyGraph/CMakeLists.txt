
add_executable(EdgeTypeRegUT test_edge_type_registry.cpp)
target_link_libraries(EdgeTypeRegUT PUBLIC PropertyGraph Catch2::Catch2WithMain Qt6::Core)
catch_discover_tests(EdgeTypeRegUT PROPERTIES LABELS "PGraph")

add_executable(NodeTypeRegUT test_node_type_registry.cpp)
target_link_libraries(NodeTypeRegUT PUBLIC PropertyGraph Catch2::Catch2WithMain Qt6::Core)
catch_discover_tests(NodeTypeRegUT PROPERTIES LABELS "PGraph")


add_executable(PGraphUT test_pgraph.cpp)
target_link_libraries(PGraphUT PUBLIC PropertyGraph Catch2::Catch2WithMain Qt6::Core)
catch_discover_tests(PGraphUT PROPERTIES LABELS "PGraph")