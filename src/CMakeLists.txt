add_subdirectory(Styles)
# add_subdirectory(Utils)
add_subdirectory(Geometry)
add_subdirectory(PropertyGraph)
# add_subdirectory(GraphCore)
add_subdirectory(LayoutEngine)
add_subdirectory(Interactors)

add_subdirectory(TerminalToolKit)
add_subdirectory(TerminalDriver)
add_subdirectory(TTKUseCases)

add_subdirectory(TerminalQt)
add_subdirectory(GraphControllers)

# add_subdirectory(FileManager)
# add_subdirectory(Persistence)
# add_subdirectory(Evidence)

IF(NOT TESTING)
add_subdirectory(GraphQt)
endif()

