add_subdirectory(Utils)
add_subdirectory(TopicGraphCore)
add_subdirectory(Evidence)
add_subdirectory(LayoutEngine)
add_subdirectory(Persistence)

IF(NOT TESTING)
add_subdirectory(Styles)
add_subdirectory(TopicGraphQt)
endif()

