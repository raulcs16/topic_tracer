add_subdirectory(Utils)
add_subdirectory(TopicGraphCore)
add_subdirectory(Evidence)
add_subdirectory(LayoutEngine)

IF(NOT TESTING)
add_subdirectory(Styles)
add_subdirectory(TopicGraphQt)
endif()

