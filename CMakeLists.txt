cmake_minimum_required(VERSION 3.27)

project(CMakeRun)

add_executable(crun ./cmake_run.cpp)
install(TARGETS crun DESTINATION ${CMAKE_SOURCE_DIR})
set_property(TARGET crun PROPERTY CXX_STANDARD 20)
