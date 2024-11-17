cmake_minimum_required(VERSION 3.13)
project(sqlightest)

set(CMAKE_C_STANDARD 23)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(TESTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")

file(GLOB_RECURSE SOURCE_FILES "${SOURCE_DIR}/*.c")

# Separate main.c for sqlite only because it has a main function and we don't
# want to include it in the tests
list(REMOVE_ITEM SOURCE_FILES "${SOURCE_DIR}/main.c")
set(MAIN_FILE "${SOURCE_DIR}/main.c")

add_executable(sqlite ${SOURCE_FILES} ${MAIN_FILE})

target_include_directories(
  sqlite
  PUBLIC ${PUBLIC_INCLUDE_DIR}
  PRIVATE ${SOURCE_DIR})

# For unit tests
file(GLOB TEST_FILES "${TESTS_DIR}/*.c")
add_executable(sqlite_tests ${SOURCE_FILES} ${TEST_FILES})

target_include_directories(
  sqlite_tests
  PUBLIC ${PUBLIC_INCLUDE_DIR}
  PRIVATE ${SOURCE_DIR})

enable_testing()
add_test(NAME sqlite_tests COMMAND sqlite_tests)
