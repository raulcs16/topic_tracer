qt_add_library(GraphQt STATIC)


set_target_properties(GraphQt PROPERTIES AUTOMOC ON)
target_link_libraries(GraphQt PRIVATE Qt6::Quick)

target_link_libraries(GraphQt
    PRIVATE Qt6::Core
    PRIVATE Qt6::Quick
    PUBLIC GraphCore
    PUBLIC Evidence
    PUBLIC LayoutEngine
    PUBLIC OGDF
    PUBLIC Persistence
)

qt_add_qml_module(GraphQt
    URI GraphQt
    VERSION 1.0
    SOURCES
        app_controller.hpp
        app_controller.cpp

        graph_store.hpp
        graph_store.cpp

        label_list_model.hpp
        label_list_model.cpp

        edge_list_model.hpp
        edge_list_model.cpp

        node_list_model.hpp
        node_list_model.cpp
         
        rect_list_model.hpp
        rect_list_model.cpp
        
        ui_states.hpp
        ui_states.cpp

        ui_context.hpp
        ui_context.cpp

        icommand.hpp
        command_context.hpp

        command_factory.hpp
        command_factory.cpp

        clear_command.hpp
        load_command.hpp
        save_command.hpp
        touch_command.hpp
        rm_command.hpp
        mv_command.hpp





        enums.hpp
        enums.cpp

    QML_FILES
        LabelListView.qml
        LabelDelegate.qml
        GraphView.qml
        CanvasView.qml
        EdgeSpline.qml
        NodeDelegate.qml
        RectDelegate.qml
        ViewPort.qml

    IMPORT_PATH 
        ${QT_QML_OUTPUT_DIRECTORY}
)

target_include_directories(GraphQt PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
        "${CMAKE_BINARY_DIR}/configured_files/include"
)
