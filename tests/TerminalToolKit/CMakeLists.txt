
add_executable(TTKUT test_tokenizer.cpp)
target_link_libraries(TTKUT PUBLIC TerminalToolKit Catch2::Catch2WithMain)
catch_discover_tests(TTKUT PROPERTIES LABELS "TerminalToolKit")

add_executable(CmdSig test_command_signature.cpp)
target_link_libraries(CmdSig PUBLIC TerminalToolKit Catch2::Catch2WithMain)
catch_discover_tests(CmdSig PROPERTIES LABELS "TerminalToolKit")

add_executable(CliUT test_cli_service.cpp)
target_link_libraries(CliUT PUBLIC TerminalToolKit Catch2::Catch2WithMain)
catch_discover_tests(CliUT PROPERTIES LABELS "TerminalToolKit")

add_executable(SEUT test_suggestion_engine.cpp)
target_link_libraries(SEUT PUBLIC TerminalToolKit Catch2::Catch2WithMain)
catch_discover_tests(SEUT PROPERTIES LABELS "TerminalToolKit")