cmake_minimum_required(VERSION 3.5.0)
project(SATsolver VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(PROJECT_BUILD_DIR ${PROJECT_ROOT_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BUILD_DIR})

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

include_directories(
  include
)

add_subdirectory(sources)
