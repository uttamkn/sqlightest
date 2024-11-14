cmake_minimum_required(VERSION 3.13)
project(sqlightest)

set(CMAKE_C_STANDARD 23)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE SOURCE_FILES "${SOURCE_DIR}/*.c")

add_executable(sqlite ${SOURCE_FILES})

target_include_directories(
  sqlite
  PUBLIC ${PUBLIC_INCLUDE_DIR}
  PRIVATE ${SOURCE_DIR})
