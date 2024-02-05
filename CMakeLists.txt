cmake_minimum_required(VERSION 3.27)

project(CMakeRun)

add_executable(crun ./cmake_run.cpp)
set_property(TARGET crun PROPERTY CXX_STANDARD 20)