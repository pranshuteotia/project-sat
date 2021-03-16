cmake_minimum_required(VERSION 3.16)
project(project_sat)

set(CMAKE_CXX_STANDARD 14)

add_executable(project-sat src/main.cpp lib/BruteForceSolver.cpp lib/BruteForceSolver.h lib/DPLLSolver.cpp lib/DPLLSolver.h obj/Clause.h src/main.cpp obj/Heuristic.cpp obj/Heuristic.h obj/DLIS.h)