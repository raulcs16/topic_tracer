qt_add_library(TopicListQt STATIC)


set_target_properties(TopicListQt PROPERTIES AUTOMOC ON)

target_link_libraries(TopicListQt
    PRIVATE Qt6::Core
    PRIVATE Qt6::Quick
)

qt_add_qml_module(TopicListQt
    URI TopicList
    VERSION 1.0
    SOURCES
        topic_list_model.hpp
        topic_list_model.cpp
    QML_FILES
        TopicListView.qml
    
    IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY}
)

target_include_directories(TopicListQt PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)
