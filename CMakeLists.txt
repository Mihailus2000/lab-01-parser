cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



option(BUILD_COVERAGE "Build coverage" OFF)
option(BUILD_TASK2 "Build 2 task" OFF)
option(BUILD_TASK3 "Build 3 task" OFF)

set(
  HUNTER_CACHE_SERVERS
  "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
  CACHE STRING "Default cache server"
)

include("tools/gate/cmake/HunterGate.cmake")

huntergate(
  URL "https://github.com/ruslo/hunter/archive/v0.23.34.tar.gz"
  SHA1 "70287b1ffa810ee4e952052a9adff9b4856d0d54"
)

project(Json_parser)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)
#find_package(Threads)



add_library(${PROJECT_NAME}
        STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/Json.cpp
)

add_executable(
        tests
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/testArray.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/testObject.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/task_3/main.cpp
        tests/testJson.cpp
)


target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(
        tests
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${GTEST_ROOT}/include
)

target_link_libraries(
        tests
        ${PROJECT_NAME}
        GTest::gtest
        GTest::main
        ${CMAKE_THREAD_LIBS_INIT}
)

enable_testing()
add_test(NAME unit_tests COMMAND tests) 

if (BUILD_TASK2)
  message("!!!!!!!!!!!!!I AM AT TASK2!!!!!!!!!!!!!")
  set(TASK2 ${PROJECT_NAME}_TASK2)
  add_executable(
          ${TASK2}
          ${CMAKE_CURRENT_SOURCE_DIR}/task_2/main.cpp
  )
  target_include_directories(
          ${TASK2}
          PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
  target_link_libraries(
          ${TASK2}
          ${PROJECT_NAME}
  )
endif()

if (BUILD_TASK3)
  message("!!!!!!!!!!!!!I AM AT TASK3!!!!!!!!!!!!!")
  hunter_add_package(nlohmann_json)
  find_package(nlohmann_json CONFIG REQUIRED)

  set(TASK3 ${PROJECT_NAME}_TASK3)

  add_executable(
          ${TASK3}
            ${CMAKE_CURRENT_SOURCE_DIR}/task_3/main.cpp
  )

  target_include_directories(
          ${TASK3}
            PUBLIC
              ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

  target_link_libraries(
          ${TASK3}
            ${PROJECT_NAME}
            nlohmann_json::nlohmann_json
  )
endif ()

if(BUILD_COVERAGE)
  set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  find_package(codecov)
  add_coverage(${PROJECT_NAME})
  add_coverage(tests)
  list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/tests/*'")
  coverage_evaluate()
endif()

