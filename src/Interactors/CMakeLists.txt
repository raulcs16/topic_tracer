add_library(Interactors STATIC
    #IONodeTypeReg
    io_node_type_reg.hpp
    create_type.hpp
    #ioPGraph
    io_pgraph.hpp
    create_node.hpp
)
target_link_libraries(Interactors 
PUBLIC PropertyGraph
PUBLIC LayoutEngine
)
target_include_directories(Interactors
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)