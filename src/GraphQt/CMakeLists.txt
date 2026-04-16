qt_add_library(GraphQt STATIC)


set_target_properties(GraphQt PROPERTIES AUTOMOC ON)
target_link_libraries(GraphQt PRIVATE Qt6::Quick)

target_link_libraries(GraphQt
    PRIVATE Qt6::Core
    PRIVATE Qt6::Quick
    PUBLIC TerminalQt
    PUBLIC TerminalQtplugin
    PUBLIC FileManagerQt
    PUBLIC GraphCore
    PUBLIC Evidence
    PUBLIC LayoutEngine
    PUBLIC OGDF
    PUBLIC Persistence
    PUBLIC GeometryQt
    PUBLIC GeometryQtplugin
)

qt_add_qml_module(GraphQt
    URI GraphQt
    VERSION 1.0
    DEPENDENCIES 
        TerminalQt
        GeometryQt
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

        boundingbox_list_model.hpp
        boundingbox_list_model.cpp


        ui_states.hpp
        ui_states.cpp

        ui_context.hpp
        ui_context.cpp

        commands/icommand.hpp
        commands/command_context.hpp
        commands/command_factory.hpp
        commands/command_factory.cpp
        
        # commands/command_autocompleter.hpp
        # commands/command_autocompleter.cpp
        commands/command_result.hpp

        commands/file_commands.hpp
        commands/graph_commands.hpp
        commands/ui_commands.hpp
    
        selection_manager.hpp
        selection_manager.cpp

        enums.hpp
        enums.cpp
    QML_FILES
        LabelListView.qml
        LabelDelegate.qml
        GraphView.qml
        CanvasView.qml
        EdgeSpline.qml
        NodeDelegate.qml
        ViewPort.qml
        BoundingBoxDelegate.qml

    IMPORT_PATH 
        ${QT_QML_OUTPUT_DIRECTORY}
)

target_include_directories(GraphQt PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
        ${CMAKE_CURRENT_SOURCE_DIR}/commands
        "${CMAKE_BINARY_DIR}/configured_files/include"
)
