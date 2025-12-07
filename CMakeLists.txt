cmake_minimum_required(VERSION 4.0)

project(shade-ir VERSION 1.0.0 LANGUAGES CXX)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(shade-ir ${SRC_DIR}/main.cpp)
