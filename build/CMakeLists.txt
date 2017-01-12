cmake_minimum_required(VERSION 3.5)

project(Strategy)
set(EXECUTABLE_NAME ${PROJECT_NAME})
set(PROJECT_SOURCE_DIR "../src/")
set(SFML_DIR "../deps/SFML-2.4.1")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")

set(CMAKE_BUILD_TYPE Release)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
configure_file(
  "${PROJECT_BINARY_DIR}/config.h.in"
  "${PROJECT_SOURCE_DIR}/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}")

link_directories("${SFML_DIR}/lib/")

add_executable(${EXECUTABLE_NAME} ${SOURCES})

include_directories("${SFML_DIR}/include/")

