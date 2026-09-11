cmake_minimum_required(VERSION 3.21)
project(sqlightest LANGUAGES C)

include(CTest)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(TESTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.c")

# Separate main.c for sqlite only because it has a main function and we don't
# want to include it in the tests
list(REMOVE_ITEM SOURCE_FILES "${SOURCE_DIR}/main.c")
set(MAIN_FILE "${SOURCE_DIR}/main.c")

add_library(sqlightest_core STATIC ${SOURCE_FILES})

target_include_directories(
  sqlightest_core
  PUBLIC ${PUBLIC_INCLUDE_DIR}
  PRIVATE ${SOURCE_DIR})

add_executable(sqlite ${MAIN_FILE})
target_link_libraries(sqlite PRIVATE sqlightest_core)

if(BUILD_TESTING)
  set(TEST_MAIN_FILE "${TESTS_DIR}/test_main.c")
  file(GLOB TEST_FILES CONFIGURE_DEPENDS "${TESTS_DIR}/*_test.c")
  add_executable(sqlite_tests ${TEST_MAIN_FILE} ${TEST_FILES})

  target_include_directories(sqlite_tests PRIVATE ${TESTS_DIR})
  target_link_libraries(sqlite_tests PRIVATE sqlightest_core)

  add_test(NAME sqlite_tests COMMAND sqlite_tests)
endif()
