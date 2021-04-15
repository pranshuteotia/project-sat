cmake_minimum_required(VERSION 3.16)
project(project_sat)
set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_library(dpll-solver-obj OBJECT lib/DPLLSolver.cpp)
add_library(brute-force-obj OBJECT lib/BruteForceSolver.cpp)
add_library(main-obj OBJECT src/main.cpp)
add_library(dlis-obj OBJECT obj/DLIS.h)
add_library(vsids-obj OBJECT obj/VSIDS.h)
add_executable(project-sat obj/Heuristic.cpp)
target_link_libraries(project-sat main-obj dpll-solver-obj brute-force-obj dlis-obj vsids-obj)