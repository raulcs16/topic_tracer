add_subdirectory(Utils)
add_subdirectory(GraphCore)
add_subdirectory(Geometry)
add_subdirectory(LayoutEngine)
add_subdirectory(FileManager)
add_subdirectory(Persistence)
add_subdirectory(Evidence)

IF(NOT TESTING)
add_subdirectory(Styles)
add_subdirectory(GraphQt)
add_subdirectory(Terminal)
endif()

