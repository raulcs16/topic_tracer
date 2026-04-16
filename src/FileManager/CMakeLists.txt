add_library(FileManager STATIC
     ifile_manager.hpp
     # file_manager_qt.hpp
     # file_manager_qt.cpp
     file_manager_dev.hpp
     file_manager_dev.cpp
)
target_link_libraries(FileManager
    PRIVATE Qt6::Core
)
target_include_directories(FileManager
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  
)
