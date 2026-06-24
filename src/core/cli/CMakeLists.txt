add_library(CLI STATIC
    token.h
    tokenizer.h
    tokenizer.cpp
    default_keys.h
    command_signature.h
    command_signature.cpp
    icommand.h
)

add_library(Core::CLI ALIAS CLI)

target_link_libraries(CLI

)
target_include_directories(CLI INTERFACE
     $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)

if(TESTING)
    include(Catch)
    file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS "*.test.cpp")
    add_executable(CLISUT ${TEST_FILES})
    target_link_libraries(CLISUT
        PUBLIC Catch2::Catch2WithMain
        PUBLIC Core::CLI
    )
    catch_discover_tests(CLISUT PROPERTIES LABELS "CLISUT")
endif()