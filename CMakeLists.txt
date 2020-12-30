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
                   COMMAND python3 -m momos build
                           -i ../src/states.c
                           -o ../test/test_states.cpp
                           --state-var ../src/states.c:current_state
                           --data-type int
                           --fn-init logic.h:logic_init
                           --fn-update logic.h:logic_run
                   WORKING_DIRECTORY ../tools
                   COMMENT "Generating state machine tests"
                   VERBATIM)

add_executable(tests test/main.cpp
                     #test/test_messages.cpp
                     test/test_states.cpp
                     test/mocks/hal_mock.cpp
                     src/messages.c
                     src/logic.c)
target_link_libraries(tests gmock
                            gtest
                            pthread)
gtest_discover_tests(tests)
