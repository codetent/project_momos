cmake_minimum_required(VERSION 3.1)

project(testy)

##
### Source definitions ###
##

include_directories("${PROJECT_SOURCE_DIR}/inc")

##
### Project ###
##

add_executable(testy src/main.c
                     src/messages.c
                     src/states.c
                     src/logic.c
                     src/hal.c)

##
### Testing ###
##

enable_testing()

find_package(GTest REQUIRED)
include(GoogleTest)

add_custom_command(OUTPUT ../test/test_states.cpp
                   COMMAND momos build
                           -i src/states.c
                           -b test/base_states.cpp
                           -o test/test_states.cpp
                   COMMENT "Generating state machine tests"
                   WORKING_DIRECTORY ..
                   VERBATIM)
execute_process(COMMAND momos include gtest
                OUTPUT_VARIABLE MOMOS_INCLUDE
                OUTPUT_STRIP_TRAILING_WHITESPACE)
message("Momos include path: ${MOMOS_INCLUDE}")

add_executable(tests test/test_states.cpp
                     test/mocks/hal_mock.cpp
                     src/messages.c
                     src/logic.c
                     ${MOMOS_INCLUDE}/momos/io.cpp)
target_include_directories(tests PRIVATE "${MOMOS_INCLUDE}"
                                         test)
target_link_libraries(tests gmock
                            gtest
                            pthread)
gtest_discover_tests(tests)
