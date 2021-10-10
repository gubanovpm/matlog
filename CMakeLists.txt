cmake_minimum_required(VERSION 2.6)
project(sat_solver)

set (SOURCES main.cpp parser.cpp lexer.cpp eval.cpp cnf.cpp)
set (HEADERS parser.hpp lexer.hpp eval.hpp cnf.hpp)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")

add_executable(sat_solver ${SOURCES})