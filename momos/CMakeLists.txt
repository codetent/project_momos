cmake_minimum_required(VERSION 3.10)

project(momos)

add_executable(momos parser.cpp)
target_link_libraries(momos PUBLIC yaml-cpp)
